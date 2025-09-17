#pragma once
#include <Arduino.h>
#include <vector>
#include "WiFiCredentials.h"

class WifiConnector {
public:
    struct Params {
        String credentials_list_file_path;
    };

    static bool init(WifiConnector::Params);
    static WifiConnector& getInstance();

    std::vector<WiFiCredentials>& get_wifi_credentials_list() { return wifi_credentials_list_; }

    // Запрещаем копирование и перемещение
    WifiConnector(const WifiConnector&) = delete;
    WifiConnector& operator=(const WifiConnector&) = delete;
    WifiConnector(WifiConnector&&) = delete;
    WifiConnector& operator=(WifiConnector&&) = delete;

    void connect_to_wifi();
    bool has_connection();

private:
    WifiConnector() = default;  
    std::vector<WiFiCredentials> load_wifi_credentials_from_config(const char* filePath);

    Params params_;
    std::vector<WiFiCredentials> wifi_credentials_list_; // ✅ правильное имя
};
