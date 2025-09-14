#include <Arduino.h>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger_helper
{
    ~Logger_helper();
    public:
    void log(const String& message) = 0;

    private:
    Logger_helper* next_logger_;
};

class SerialLogger : public Logger_helper
{
    public:
    SerialLogger(Logger_helper* next = nullptr);
    
    
    void log(const String& message) override;   
    private:
    void internal_log(const String& message);
}

class Logger{
public:
    Logger();
    ~Logger();

public:
    static void log(LogLevel level, const String& message) {

        switch (level) {
            case LogLevel::DEBUG:
                Serial.print("[DEBUG] ");
                break;
            case LogLevel::INFO:
                Serial.print("[INFO] ");
                break;
            case LogLevel::WARN:
                Serial.print("[WARN] ");
                break;
            case LogLevel::ERROR:
                Serial.print("[ERROR] ");
                break;
        }
        Serial.println(message);
    }

private:
    init_Logger_helpers();
    Logger_helper* head_logger_;
}