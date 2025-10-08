#pragma once
#include <string>
#include <Arduino.h>
#include <time.h>

class TimeBase {
public:
    virtual ~TimeBase() {}

    // Установить текущее время (например, от сервера)
    virtual void setTime(time_t epochTime) = 0;

    // Вернуть текущее время (Epoch seconds)
    virtual time_t getCurrentTime() const = 0;

    // Вернуть текущее время в виде строки (удобно для логов)
    virtual std::string getFormattedTime() const = 0;
};


TimeBase* get_time_object_with_ownership();

