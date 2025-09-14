#include "device_manager.h"

DeviceManager::DeviceManager():devices_head_(nullptr) {
    init_devices();
}

void DeviceManager::init_devices() {
    // Инициализация устройств и цепочки ответственности
    LocalDevice* device1 = new LocalDevice(nullptr);
    device1->setup(D1); // Пин D1 (GPIO5)

    LocalDevice* device2 = new LocalDevice(device1);
    device2->setup(D2); // Пин D2 (GPIO4)

    LocalDevice* device3 = new LocalDevice(device2);
    device3->setup(D5); // Пин D5 (GPIO14)

    LocalDevice* device4 = new LocalDevice(device3);
    device4->setup(D6); // Пин D6 (GPIO12)

    LocalDevice* device5 = new LocalDevice(device4);
    device5->setup(D7); // Пин D7 (GPIO13)

    devices_head_ = device5; // Голова цепочки
}

void DeviceManager::handele_job(const JobParams& job) {
    if(devices_head_) {
        devices_head_->handleJob(job);
    }
}

void DeviceManager::print_state() {
    if(devices_head_) {
        StaticJsonDocument<2048> doc;
        JsonArray arr = doc.to<JsonArray>();

        devices_head_->collect_states(arr);

        serializeJson(arr, Serial);
        Serial.println(); 
    }
}