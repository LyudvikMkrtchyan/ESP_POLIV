#pragma once
#include <Arduino.h>
#include <vector>

class WiFiCredentials {
public:
    WiFiCredentials() {}
    WiFiCredentials(const String& s, const String& p) : ssid_(s), password_(p) {}

    String get_ssid() const { return ssid_; }
    String get_password() const { return password_; }

    void set_ssid(String ssid) { ssid_ = ssid; }
    void set_password(String password) { password_ = password; }

    // 🔹 Метод для печати информации
    void print_WiFiCredentials_info() const {
        Serial.print("SSID: ");
        Serial.println(ssid_);
        Serial.print("Password: ");
        Serial.println(password_);
    }

private:
    String ssid_;
    String password_;
};

// Объявления функций загрузки
std::vector<WiFiCredentials> load_wifi_credentials_from_file(const String& filename);
std::vector<WiFiCredentials> load_wifi_credentials_from_json(const String& json);
