#pragma once
#include <Arduino.h>

#include "WiFiCredentials.h"


class WifiConnector {
public:
    struct Params {
        String cridentals_list_file_path;
    };

    WifiConnector(const WifiConnector::Params& params);
    bool has_connection(const std::string& module_name);
    void connect_to_wifi();

private:
    std::vector<WiFiCredentials> load_wifi_credentials_from_config(const char* filePath);

    private:
    Params params_;
    std::vector<WiFiCredentials> wifi_cridentals_list_;

};