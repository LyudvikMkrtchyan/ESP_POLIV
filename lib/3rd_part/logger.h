#pragma once
#include <Arduino.h>
#include <string>

class Logger {
public:
    static void log_info(const String& message);
    static void log_warning(const String& message);
    static void log_error(const String& message);

    static void log_info(const std::string& message);
    static void log_warning(const std::string& message);
    static void log_error(const std::string& message);

     static void log_info(const char* message);
    static void log_warning(const char* message);
    static void log_error(const char* message);

private:
    Logger() = default;
    ~Logger() = default;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};
