#pragma once
enum DeviceType {
    UNKNOWN,
    GREEN,
    REED,
    REMOTE,
};

struct DeviceTypeHelper {
    static std::string toString(DeviceType type) {
        switch (type) {
            case DeviceType::GREEN:  return "GREEN";
            case DeviceType::REED:   return "REED";
            case DeviceType::REMOTE: return "REMOTE";
            default:                 return "UNKNOWN";
        }
    }

    static DeviceType fromString(const std::string& str) {
        if (str == "GREEN")  return DeviceType::GREEN;
        if (str == "REED")   return DeviceType::REED;
        if (str == "REMOTE") return DeviceType::REMOTE;
        return DeviceType::UNKNOWN;
    }
};
