#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WifiConnector.h>
#include <WiFiCredentials.h>
#include <backend_server.h>
#include <job_params.h>
#include "device_manager.h"


#define GREEN_LAMP D5


// MQTT параметры
const char* mqtt_server = "86.107.197.36";   // IP твоего брокера
const int mqtt_port = 1883;                  // стандартный порт MQTT
const char* mqtt_topic = "poliv/commands";         // топик, который слушаешь

WiFiClient espClient;
PubSubClient client(espClient);
JobParams currentJob;  // текущая задача
bool jobReceived = false; // флаг, что задача получена
DeviceManager device_manager;

// Коллбэк — вызывается при получении сообщения
void callback(char* topic, byte* payload, unsigned int length) {
  
  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }


  //Serial.println("message: " + msg);

  if (currentJob.fromJson(msg)) {
    Serial.println("Новая задача получена!");
    Serial.println("" + currentJob.toJson());
    jobReceived = true;
  }
}


void setup() {
  Serial.begin(9600);
  initalizePins();
  setup_wifi();
  setup_backend_servers();



  // Настройка MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  auto backend_servers = BackendServers::getInstance();
  backend_server::RequestParams request_params;
  request_params.type = backend_server::RequestType::POST;
  request_params.endpoint = "ping";
  request_params.body = "{\"ping\": \"pong\"}";
  backend_servers.send_request(request_params);
}

// Функция для переподключения к MQTT, если связь упала
void reconnect() {
  while (!client.connected()) {
    Serial.print("Подключаюсь к MQTT...");
    if (client.connect("ESP8266Client")) {  // client ID должен быть уникальным
      Serial.println("успешно!");
      client.subscribe(mqtt_topic);
    

        digitalWrite(GREEN_LAMP, RELAY_ON);
        digitalWrite(D1, RELAY_ON);
        digitalWrite(D2, RELAY_ON);
        delay(500);
        digitalWrite(GREEN_LAMP, RELAY_OFF);
        digitalWrite(D1, RELAY_OFF);
        digitalWrite(D2, RELAY_OFF);
        delay(500);

         digitalWrite(GREEN_LAMP, RELAY_ON);
        digitalWrite(D1, RELAY_ON);
        digitalWrite(D2, RELAY_ON);
        delay(500);
        digitalWrite(GREEN_LAMP, RELAY_OFF);
        digitalWrite(D1, RELAY_OFF);
        digitalWrite(D2, RELAY_OFF);
        delay(500);

         digitalWrite(GREEN_LAMP, RELAY_ON);
        digitalWrite(D1, RELAY_ON);
        digitalWrite(D2, RELAY_ON);
        delay(500);
        digitalWrite(GREEN_LAMP, RELAY_OFF);
        digitalWrite(D1, RELAY_OFF);
        digitalWrite(D2, RELAY_OFF);
        delay(500);



  
    } else {
      Serial.print("ошибка, rc=");
      Serial.print(client.state());
      Serial.println(" пробую снова через 5 сек");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 
  
  if(jobReceived)
  {
    //currentJob.print();
    device_manager.handele_job(currentJob);

    currentJob = JobParams(); //
    jobReceived = false;
    device_manager.print_state();
  }
}
