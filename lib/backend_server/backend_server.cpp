#include <backend_server.h>
#include <logger.h>
#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <vector>
#include <string>
#include <ESP8266WiFi.h>
#include "backend_server_impl_inherits.h"
#include "backend_server_params.h"


 BackendServers& BackendServers::getInstance() {
    static BackendServers instance;
    return instance;
}



void BackendServers::init(const std::string& backend_server_config_file_path) 

{
    BackendServers& instance = BackendServers::getInstance();

    instance.init_internal(backend_server_config_file_path);

}

void BackendServers::send_request(const backend_server::RequestParams& request_params) {
    if (servers_.empty()) {
        Logger::log_warning("No backend servers available to send request.");
        return;
    }   

    // Отправляем запрос на текущий сервер
    servers_[curent_server_index_]->send_request(request_params);
    for(auto it = servers_.begin(); it != servers_.end(); ++it) {
        (*it)->send_request(request_params);
    }

    // Переходим к следующему серверу в списке (круговой обход)
}

void BackendServers::init_internal(const std::string& backend_server_config_file_path) {


    backend_server::BackendConfig config = this->parse_config_file(backend_server_config_file_path);
    output_file_name_ = std::string(config.output_file_name.c_str());
    if (config.servers.empty()) {
        Logger::log_warning("No backend servers loaded from config.");
        return;
    }

    for (const auto& server_base : config.servers) {
        if (server_base->type == "HTTP") {
            servers_.push_back(new HttpServer(server_base->params));
            Logger::log_info("Initialized HTTP server.");
        } else {
            Logger::log_warning("Unknown server type: " + server_base->type);
        }
    }

    if (servers_.empty()) {
        Logger::log_warning("No valid backend servers initialized.");
        return;
    }

    Logger::log_info("BackendServers initialized with " + String(servers_.size()) + " servers.");
}

backend_server::BackendConfig BackendServers::parse_config_file(std::string backend_server_config_file_path)
{
    backend_server::BackendConfig config;

    if (!LittleFS.begin()) {
        Logger::log_error("Failed to mount file system in BackendServers::init");
        return config;
    }

    File file = LittleFS.open(backend_server_config_file_path.c_str(), "r");
    if (!file) {
        Logger::log_error("Failed to open config file: " + String(backend_server_config_file_path.c_str()));
        return config;
    }

    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        Logger::log_error("JSON parse error: " + String(error.c_str()));
        return config;
    }

    // читаем строку
    config.output_file_name = doc["output_file_name"].as<String>();
    Logger::log_info("Output file name: " + config.output_file_name);

    // читаем массив серверов
    JsonArray arr = doc["servers"].as<JsonArray>();
    for (JsonObject obj : arr) {
        backend_server::BackendServerImplParams* server_params = new backend_server::BackendServerImplParams();
        server_params->type = obj["type"].as<String>();
        server_params->params.port = obj["params"]["port"].as<int>();
        server_params->params.ip_address = obj["params"]["ip_address"].as<String>();

        Logger::log_info("Loaded backend server:");
        Logger::log_info("  type = " + server_params->type);
        Logger::log_info("  port = " + String(server_params->params.port));
        Logger::log_info("  ip   = " + server_params->params.ip_address);

        config.servers.push_back(server_params);
    }

    return config;
}

