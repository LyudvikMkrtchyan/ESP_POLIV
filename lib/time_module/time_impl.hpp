#pragma once



#include "time_base.hpp"

class BackendTime : public TimeBase {
private:
    time_t offset_; // разница между серверным временем и millis()
    unsigned long syncMillis_;

public:
    BackendTime(time_t serverEpochTime);

    void setTime(time_t epochTime) override;

    time_t getCurrentTime() const override;

    std::string getFormattedTime() const override;
};

