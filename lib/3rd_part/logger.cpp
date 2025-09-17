#include "logger.h"

void Logger::log_info(const String& message) {
    Serial.println("[INFO] " + message);
}

void Logger::log_warning(const String& message) {
    Serial.println("[WARNING] " + message);
}
void Logger::log_error(const String& message) {
    Serial.println("[ERROR] " + message);
}