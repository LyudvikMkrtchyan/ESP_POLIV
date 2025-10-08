#include "logger.h"

//.. const String& implementation
void Logger::log_info(const String& message) 
{
    Serial.println("[INFO] " + message);
}

void Logger::log_warning(const String& message) 
{
    Serial.println("[WARNING] " + message);
}
void Logger::log_error(const String& message) 
{
    Serial.println("[ERROR] " + message);
}
void Logger::log_debug(const String& message) 
{
    Serial.println("[DEBUG] " + message);
}


//.. cont std::string& implemantation 
void Logger::log_info(const std::string& message) 
{
    log_info(String(message.c_str()));
}

void Logger::log_warning(const std::string& message) 
{
    log_warning(String(message.c_str()));
}

void Logger::log_error(const std::string& message) 
{
    log_error(String(message.c_str()));
}

void Logger::log_debug(const std::string& message) 
{
    log_debug(String(message.c_str()));
}


//.. const char* implementacion 
void Logger::log_info(const char* message)
{
    log_info(String(message));
}

void Logger::log_warning(const char* message)
{
    log_warning(String(message));

}


void Logger::log_error(const char* message)
{
    log_error(String(message));

}

void Logger::log_debug(const char* message) 
{
    log_debug(String(message));

}