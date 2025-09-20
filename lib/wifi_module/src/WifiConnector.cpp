#include <logger.h>
#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <vector>
#include <string>
#include <ESP8266WiFi.h>


#include "WifiConnector.h"
#include "WiFiCredentials.h"

bool WifiConnector::init(WifiConnector::Params params) {
    WifiConnector& instance = getInstance();
    instance.params_ = params;

    instance.wifi_credentials_list_ =
        instance.load_wifi_credentials_from_config(instance.params_.credentials_list_file_path.c_str());

    return !instance.wifi_credentials_list_.empty();
}


WifiConnector& WifiConnector::getInstance() {
    static WifiConnector instance;
    return instance;
}

std::vector<WiFiCredentials> WifiConnector::load_wifi_credentials_from_config(const char* filePath) {
    std::vector<WiFiCredentials> creds;

    if (!LittleFS.begin()) {
        Serial.println(F("Failed to mount file system"));
        return creds;
    }

    File file = LittleFS.open(filePath, "r");
    if (!file) {
        Serial.println(F("Failed to open config file"));
        Serial.println(filePath);
        return creds;
    }


    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, file);

    if (error) {
        Serial.print(F("JSON parse error: "));
        Serial.println(error.c_str());
        return creds;
    }

    JsonArray arr = doc.as<JsonArray>();
    for (JsonObject obj : arr) {
        String ssid = obj["ssid"].as<String>();
        String password = obj["password"].as<String>();
        creds.emplace_back(ssid, password);
    }

    file.close();
    return creds;
}

bool WifiConnector::has_connection() {
    
    int reconect_attempts = 5;
    while(reconect_attempts)
        {
        if(WiFi.status() == WL_CONNECTED) {
            return true;
        }
        connect_to_wifi();
    }
    return false;
}



void WifiConnector::connect_to_wifi() {
    if (wifi_credentials_list_.empty()) {
        Serial.println(F("[WiFi] No credentials loaded."));
        return;
    }

    // 🔎 Сканирование всех доступных сетей
    Serial.println(F("[WiFi] Scanning for available networks..."));
    int networksFound = WiFi.scanNetworks();
    if (networksFound == 0) {
        Serial.println(F("[WiFi] No networks found."));
        return;
    }

    Serial.print(F("[WiFi] Found "));
    Serial.print(networksFound);
    Serial.println(F(" networks:"));

    // Вывод списка сетей
    for (int i = 0; i < networksFound; i++) {
        Serial.print("  ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (RSSI: ");
        Serial.print(WiFi.RSSI(i));
        Serial.println(" dBm)");
    }

    // 🚀 Попытка подключения к сетям из списка, если они есть в найденных
    for (int i = 0; i < networksFound; i++) {
        String ssid = WiFi.SSID(i);

        // проверка: есть ли эта сеть в нашем списке
        for (const auto& cred : wifi_credentials_list_) {
            if (ssid == cred.get_ssid()) {
                Serial.println(F("==================================="));
                Serial.print(F("[WiFi] Trying SSID: "));
                Serial.println(ssid);

                WiFi.mode(WIFI_STA);
                WiFi.begin(cred.get_ssid().c_str(), cred.get_password().c_str());

                unsigned long startAttemptTime = millis();
                const unsigned long timeout = 15000;

                while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
                    delay(500);
                    Serial.print(F("."));
                }
                Serial.println();

                if (WiFi.status() == WL_CONNECTED) {
                    Serial.println(F("[WiFi] Connected successfully!"));
                    Serial.print(F("[WiFi] SSID: "));
                    Serial.println(WiFi.SSID());
                    Serial.print(F("[WiFi] IP Address: "));
                    Serial.println(WiFi.localIP());
                    Serial.print(F("[WiFi] Signal strength (RSSI): "));
                    Serial.print(WiFi.RSSI());
                    Serial.println(F(" dBm"));
                    Serial.println(F("==================================="));
                    return;
                } else {
                    Serial.print(F("[WiFi] Failed to connect to SSID: "));
                    Serial.println(ssid);
                }
            }
        }
    }

    Serial.println(F("[WiFi] Could not connect to any configured network."));
}



