#include "utils.h"
#include "readers_base.hpp"
#include <Arduino.h>
#include <set>
#include <WifiConnector.h>
#include <backend_server.h>
#include <logger.h>
#include <string>


std::map <int, int> pinToName = {
    {1, D1},
    {2, D2},
    {3, D5},
    {4, D6},
    {5, D7}
};

std::map <int, int> nameToPin = {
    {D1, 1},
    {D2, 2},
    {D5, 3},
    {D6, 4},
    {D7, 5}
};


void initalizePins() {
    
        pinMode(D1, OUTPUT);
        pinMode(D2, OUTPUT);
        pinMode(D5, OUTPUT);
        pinMode(D6, OUTPUT);
        pinMode(D7, OUTPUT);


        digitalWrite(D1, RELAY_OFF);
        digitalWrite(D2, RELAY_OFF);
        digitalWrite(D5, RELAY_OFF);
        digitalWrite(D6, RELAY_OFF);
        digitalWrite(D7, RELAY_OFF);
}

int get_internal_pin(const std::string &device_name) {


    if (device_name == "device_1") return D1;
    if (device_name == "device_2") return D2;
    if (device_name == "device_3") return D5;
    if (device_name == "device_4") return D6;
    if (device_name == "device_5") return D7;
    Logger::log_error("Unknown device name: " + String(device_name.c_str()));
    return -1;
}

int get_external_pin(const int name) {
    auto it = nameToPin.find(name);
    if (it != nameToPin.end()) {
         return it->second;  // 👉 value
    }
    return -1; // not found
}

void setup_wifi() {
      // WiFi подключение через твой класс
  WifiConnector::Params wifi_connector_params;
  
  wifi_connector_params.credentials_list_file_path = "/config_files/wifi_credentals.json";

  WifiConnector::init(wifi_connector_params);
  WifiConnector::getInstance().connect_to_wifi();

    if (WifiConnector::getInstance().has_connection("Setup")) {
        Logger::log_info("WiFi connected successfully.");
    } else {
        Logger::log_warning(String("Failed to connect to WiFi. [") +
                    __FILE__ + ":" + String(__LINE__) + "]");

    }

}
void setup_backend_servers() {
    // BackendServers подключение через твой класс

    atl::File backend_server_config_file;
    backend_server_config_file.file_path =  "/config_files/backend_servers_config.json";
    backend_server_config_file.file_type = atl::FileType::LOCAL_FILE;
  
    std::string backend_server_config_file_path = "/config_files/backend_servers_config.json";


    Logger::log_info("Initializing Backend Servers with config file: " + String(backend_server_config_file_path.c_str()));
    BackendServers::init(backend_server_config_file_path);

}