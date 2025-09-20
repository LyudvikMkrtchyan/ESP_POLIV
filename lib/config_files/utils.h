#pragma once

#include <map>
#include <string>
#include <Arduino.h>

#define RELAY_ON  HIGH
#define RELAY_OFF LOW



int get_internal_pin(const std::string &device_name);
int get_external_pin(const int internal_pin);

void setup_wifi();
void setup_backend_servers();

void initalizePins();