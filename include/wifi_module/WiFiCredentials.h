#pragma once
#include <Arduino.h>



class WiFiCredentials {
public:

    WiFiCredentials() {}
    WiFiCredentials(const String& s, const String& p) : ssid_(s), password_(p) {}

    String get_ssid() {    return ssid_;}
    String get_password() {    return password_;}


    void set_ssid(String ssid) 
    {    
        ssid_ = ssid;
    }
    void set_password(String password) 
    {    
        password_ = password;
    }

private:
    String ssid_;
    String password_;
};

//std::vector<WiFiCredentials> load_wifi_credentials_from_file(const String& filename);
//std::vector<WiFiCredentials> load_wifi_credentials_from_json(const String& json);
String load_wifi_credentials_from_config();
