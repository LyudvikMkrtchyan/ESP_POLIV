#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WifiConnector.h>
#include <WiFiCredentials.h>
#include <backend_server.h>
#include <job_params.h>
#include "device_manager.h"
#include <string.h>
#include "utils.h"



#define GREEN_LAMP D5


// MQTT параметры
const char* mqtt_server = "86.107.197.36";   // IP твоего брокера
const int mqtt_port = 1883;                  // стандартный порт MQTT
const char* mqtt_topic = "poliv/commands";         // топик, который слушаешь

WiFiClient espClient;
PubSubClient client(espClient);
JobParams currentJob;  // текущая задача
bool jobReceived = false; // флаг, что задача получена
DeviceManager* device_manager;


void setup() {
  Serial.begin(9600);
  initalizePins();
  setup_wifi();
  setup_backend_servers();

  // Mqtt broker parmas


  MqttManagerBase::Params mqtt_params_;

mqtt_params_.clientId = "ESP_MAIN";
mqtt_params_.host = "86.107.197.36";
mqtt_params_.port = 1883;
mqtt_params_.keepAlive = 60;
mqtt_params_.cleanSession = true;
mqtt_params_.username = "myUser";
mqtt_params_.password = "myPassword";



  device_manager = new DeviceManager(mqtt_params_);

  auto backend_servers = BackendServers::getInstance();
  backend_server::RequestParams request_params;
  request_params.type = backend_server::RequestType::POST;
  request_params.endpoint = "ping";
  request_params.body = "{\"ping\": \"pong\"}";
  backend_servers.send_request(request_params);
}



void loop(){
    device_manager->loop();
}
