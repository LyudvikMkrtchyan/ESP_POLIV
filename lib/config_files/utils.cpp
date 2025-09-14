#include "utils.h"
#include <Arduino.h>


std::map <int, int> pinToName = {
    {1, D1},
    {2, D2},
    {3, D5},
    {4, D6},
    {5, D7}
};

std::map <int, int> nameToPin = {
    {D1, 1},
    {D2, 2},
    {D5, 3},
    {D6, 4},
    {D7, 5}
};


void initalizePins() {
    
        pinMode(D1, OUTPUT);
        pinMode(D2, OUTPUT);
        pinMode(D5, OUTPUT);
        pinMode(D6, OUTPUT);
        pinMode(D7, OUTPUT);


        digitalWrite(D1, RELAY_OFF);
        digitalWrite(D2, RELAY_OFF);
        digitalWrite(D5, RELAY_OFF);
        digitalWrite(D6, RELAY_OFF);
        digitalWrite(D7, RELAY_OFF);
}

int get_internal_pin(const int pin) {
    auto it = pinToName.find(pin);
    if (it != pinToName.end()) {
         return it->second;  // 👉 value
    }
    return -1; // not found
}

int get_external_pin(const int name) {
    auto it = nameToPin.find(name);
    if (it != nameToPin.end()) {
         return it->second;  // 👉 value
    }
    return -1; // not found
}