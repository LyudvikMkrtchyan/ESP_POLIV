#include "backend_server_impl_inherits.h"
#include "backend_server_params.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "logger.h"
#include <string>


/// ResponseBase Implementation

ResponseBase::ResponseBase(std::string response_body):response_body_(response_body){}

///HttpServerResponse Implementation

HttpServerResponse::HttpServerResponse(std::string response_body):ResponseBase(response_body){}


std::string HttpServerResponse::get_response_body()
{
    return response_body_;
}



/// HttpServer Implementacion
HttpServer::HttpServer(const backend_server::BackendServerParams& params) : params_(params){}

HttpServerResponse* HttpServer::send_request(const backend_server::RequestParams& request_params) {
    Logger::log_debug("HttpServer::send_request is runned");
    
    if (WiFi.status() != WL_CONNECTED) {
        Logger::log_error("WiFi not connected, cannot send HTTP request");
        return nullptr;
    }

    WiFiClient client;
    HTTPClient http;

    String url = "http://" + params_.ip_address + ":" + String(params_.port) + "/" + request_params.endpoint;
    Logger::log_info("Sending HTTP request to: " + url);

    if (!http.begin(client, url)) {
        Logger::log_error("Failed to begin HTTP connection");
        return nullptr;
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
            return nullptr;
    }

    std::string response_body = "";
    if (httpCode > 0) {
        Logger::log_debug("HTTP Response code: " + String(httpCode));
        response_body = http.getString().c_str();
        Logger::log_debug("HTTP Response body: " + response_body);
    } else {
        Logger::log_error("HTTP Request failed, error: " + String(http.errorToString(httpCode).c_str()));
    }
    HttpServerResponse* response = new HttpServerResponse(response_body);
    
    http.end();
    return response;
}
