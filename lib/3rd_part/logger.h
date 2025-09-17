#pragma once
#include <Arduino.h>

class Logger {
public:
    static void log_info(const String& message);
    static void log_warning(const String& message);
    static void log_error(const String& message);

private:
    Logger() = default;
    ~Logger() = default;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};
