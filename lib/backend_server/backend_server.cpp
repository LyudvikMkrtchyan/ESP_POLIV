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


 BackendServers& BackendServers::getInstance() 
 {
    Logger::log_debug("BackendServers::getInstance is runned .");

    static BackendServers instance;
    return instance;
}



void BackendServers::init(const atl::File& backend_server_config_file) 

{
    Logger::log_debug("BackendServers::init is runned .");
    BackendServers& instance = BackendServers::getInstance();

    instance.init_internal(backend_server_config_file);

}

ResponseBase* BackendServers::send_request(const backend_server::RequestParams& request_params) 
{
    if (servers_.empty()) 
    {
        Logger::log_warning("No backend servers available to send request.");
        return nullptr;
    }   

    // Отправляем запрос на текущий сервер
    servers_[curent_server_index_]->send_request(request_params);
    for(auto it = servers_.begin(); it != servers_.end(); ++it) 
    {
        ResponseBase* tmp_response;
        tmp_response =  (*it)->send_request(request_params);
        if(tmp_response)
        {
            return tmp_response;
        }
    }

    return nullptr;

    // Переходим к следующему серверу в списке (круговой обход)
}

void BackendServers::init_internal(const atl::File& backend_server_config_file) 
{

    Logger::log_debug("BackendServers::init_internal is runned .");
    backend_server::BackendConfig config = this->parse_config_file(backend_server_config_file);
    output_file_name_ = std::string(config.output_file_name.c_str());
    if (config.servers.empty()) 
    {
        Logger::log_warning("No backend servers loaded from config.");
        return;
    }

    for (const auto& server_base : config.servers) 
    {
        if (server_base->type == "HTTP") 
        {
            servers_.push_back(new HttpServer(server_base->params));
            Logger::log_info("Initialized HTTP server.");
        } 
        else 
        {
            Logger::log_warning("Unknown server type: " + server_base->type);
        }
    }

    if (servers_.empty()) 
    {
        Logger::log_warning("No valid backend servers initialized.");
        return;
    }

    Logger::log_info("BackendServers initialized with " + String(servers_.size()) + " servers.");
}

backend_server::BackendConfig BackendServers::parse_config_file(const atl::File& backend_server_config_file)
{
    Logger::log_debug("BackendServers::parse_config_file is runned .");

    backend_server::BackendConfig config;

    // if (!LittleFS.begin()) {
    //     Logger::log_error("Failed to mount file system in BackendServers::init");
    //     return config;
    // }

    // File file = LittleFS.open(backend_server_config_file_path.c_str(), "r");
    // if (!file) {
    //     Logger::log_error("Failed to open config file: " + backend_server_config_file.file_path);
    //     return config;
    // }

    StaticJsonDocument<2048> doc;

    std::string backend_file_content = read_file_content(backend_server_config_file);
    Logger::log_debug("backend_file_content = " + backend_file_content);

    DeserializationError error = deserializeJson(doc, backend_file_content);
    //file.close();

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

