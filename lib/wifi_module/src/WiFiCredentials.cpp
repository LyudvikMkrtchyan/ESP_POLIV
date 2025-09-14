#include "../include/WiFiCredentials.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <LittleFS.h>
#include <vector>

std::vector<WiFiCredentials> load_wifi_credentials_from_json(const String& json) {
    std::vector<WiFiCredentials> creds;

    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        Serial.print("JSON parse error: ");
        Serial.println(error.c_str());
        return creds; // пустой вектор
    }

    JsonArray arr = doc.as<JsonArray>();
    for (JsonObject obj : arr) {
        String ssid = obj["ssid"].as<String>();
        String password = obj["password"].as<String>();
        creds.emplace_back(ssid, password);
    }

    return creds;
}

std::vector<WiFiCredentials> load_wifi_credentials_from_config(const char* filename) {
    std::vector<WiFiCredentials> creds;

    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed!");
        return creds;
    }

    File file = LittleFS.open(filename, "r");
    if (!file) {
        Serial.print("Не удалось открыть файл: ");
        Serial.println(filename);
        return creds;
    }

    String json;
    while (file.available()) {
        json += (char)file.read();
    }
    file.close();

    return load_wifi_credentials_from_json(json);
}
