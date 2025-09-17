#pragma once

#include <map>
#include <Arduino.h>

#define RELAY_ON  HIGH
#define RELAY_OFF LOW



int get_internal_pin(const int external_pin);
int get_external_pin(const int internal_pin);

void setup_wifi();
void setup_backend_servers();

void initalizePins();