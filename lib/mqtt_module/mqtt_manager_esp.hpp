
#pragma once
#include "mqtt_manager_base.hpp"
#include "WifiConnector.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class MqttManagerEsp : public MqttManagerBase {
private:
    WiFiClient espClient_;
    PubSubClient mqttClient_;
    

    int wifi_reconnect_timeout_ = 10;             // сколько секунд ждать WiFi
    std::string module_name_ = "MqttManagerEsp";  // имя модуля для логов

public:
    explicit MqttManagerEsp(const Params& params);

    void connect() override;
    void disconnect() override;
    void subscribe(const std::string& topic, int qos=1) override;
    void publish(const std::string& topic, const std::string& payload, int qos=1, bool retain=false) override;
    bool is_connected() override;
    void loop();

    // сеттеры для параметров
    void set_wifi_reconnect_time(int sec) { wifi_reconnect_timeout_ = sec; }
    void set_module_name(const std::string& name) { module_name_ = name; }
};
