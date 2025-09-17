#include "backend_server_impl_inherits.h"
#include "backend_server_params.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "logger.h"

HttpServer::HttpServer(const backend_server::BackendServerParams& params) : params_(params){}



void HttpServer::send_request(const backend_server::RequestParams& request_params) {
    if (WiFi.status() != WL_CONNECTED) {
        Logger::log_error("WiFi not connected, cannot send HTTP request");
        return;
    }

    WiFiClient client;
    HTTPClient http;

    // Формируем URL
    String url = "http://" + params_.ip_address + ":" + String(params_.port) + "/" + request_params.endpoint;

    Logger::log_info("Sending HTTP request to: " + url);

    if (!http.begin(client, url)) {
        Logger::log_error("Failed to begin HTTP connection");
        return;
    }

    int httpCode = -1;

    switch (request_params.type) {
        case backend_server::RequestType::GET:
            httpCode = http.GET();
            break;

        case backend_server::RequestType::POST:
            http.addHeader("Content-Type", "application/json");
            httpCode = http.POST(request_params.body);
            break;

        case backend_server::RequestType::PUT:
            http.addHeader("Content-Type", "application/json");
            httpCode = http.PUT(request_params.body);
            break;

        case backend_server::RequestType::DELETE:
            httpCode = http.sendRequest("DELETE", request_params.body);
            break;

        default:
            Logger::log_warning("Unknown HTTP request type");
            http.end();
            return;
    }

    if (httpCode > 0) {
        Logger::log_info("HTTP Response code: " + String(httpCode));
        String payload = http.getString();
        Logger::log_info("Response: " + payload);
    } else {
        Logger::log_error("HTTP Request failed, error: " + String(http.errorToString(httpCode).c_str()));
    }

    http.end();
}
