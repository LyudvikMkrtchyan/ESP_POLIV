#include "WifiConnector.h"
#include "mqtt_manager_esp.hpp"

MqttManagerEsp::MqttManagerEsp(const Params& params)
    : MqttManagerBase(params), mqttClient_(espClient_)
{
    mqttClient_.setServer(params_.host.c_str(), params_.port);
    mqttClient_.setCallback([this](char* topic, byte* payload, unsigned int length) {
        std::string pl((char*)payload, (char*)payload + length);
        this->dispatch_message(topic, pl);
    });
}

void MqttManagerEsp::connect() {
    if (!WifiConnector::getInstance().has_connection(module_name_)) {
        Serial.println(F("[MQTT] No WiFi connection available"));
        return;
    }

    while (!mqttClient_.connected()) {
        bool connected;
        if (params_.username.empty()) {
            connected = mqttClient_.connect(params_.clientId.c_str());
        } else {
            connected = mqttClient_.connect(params_.clientId.c_str(),
                                            params_.username.c_str(),
                                            params_.password.c_str());
        }

        if (connected) {
            Serial.println(F("[MQTT] Connected to broker"));
        } else {
            Serial.print(F("[MQTT] Failed to connect, rc="));
            Serial.println(mqttClient_.state());
            delay(2000);
        }
    }
}

void MqttManagerEsp::disconnect() {
    mqttClient_.disconnect();
}

void MqttManagerEsp::subscribe(const std::string& topic, int qos) {
    if (!WifiConnector::getInstance().has_connection(module_name_)) return;
    mqttClient_.subscribe(topic.c_str());
}

void MqttManagerEsp::publish(const std::string& topic, const std::string& payload, int qos, bool retain) {
    if (!WifiConnector::getInstance().has_connection(module_name_)) return;
    mqttClient_.publish(topic.c_str(), payload.c_str(), retain);
}

bool MqttManagerEsp::is_connected() {
    return mqttClient_.connected();
}

void MqttManagerEsp::loop() {
    if (!WifiConnector::getInstance().has_connection(module_name_)) return;
    mqttClient_.loop();
}
