#pragma once
#include <string>
#include <functional>
#include <unordered_map>

class MqttManagerBase {
public:
    struct Params {
        std::string clientId;
        std::string host;
        int port = 1883;
        int keepAlive = 60;
        bool cleanSession = true;
        std::string username;
        std::string password;
    };

    using MessageCallback = std::function<void(const std::string& topic,
                                               const std::string& payload)>;

protected:
    Params params_;
    std::unordered_map<std::string, MessageCallback> topicCallbacks_;

public:
    explicit MqttManagerBase(const Params& params) : params_(params) {}
    virtual ~MqttManagerBase() {}

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void subscribe(const std::string& topic, int qos=1) = 0;
    virtual void publish(const std::string& topic, const std::string& payload, int qos=1, bool retain=false) = 0;
    virtual bool is_connected() = 0;
    virtual void loop() = 0;

    // назначение колбэка для конкретного топика
    void set_topic_callback(const std::string& topic, MessageCallback cb) {
        topicCallbacks_[topic] = cb;
    }

    // вызов обработчика (используется внутри реализации)
    void dispatch_message(const std::string& topic, const std::string& payload) {
        auto it = topicCallbacks_.find(topic);
        if (it != topicCallbacks_.end()) {
            it->second(topic, payload);
        }
    }

    const Params& get_params() const { return params_; }
};
