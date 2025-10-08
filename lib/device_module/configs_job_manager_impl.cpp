#include "configs_job_manager_impl.hpp"
#include "WifiConnector.h"
#include "logger.h"
#include "utils.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

void ConfigJobsManager::run_configs_loop()
{
    this->monitor_wifi_battery_status();
}

void ConfigJobsManager::monitor_wifi_battery_status()
{
    Logger::log_debug(" ConfigJobsManager::monitor_wifi_battery_status() is runned .");
    
    if(!WifiConnector::getInstance().has_connection("ConfigJobsManager::monitor_wifi_battery_status"))
    {
        Logger::log_debug(" !WifiConnector::getInstance().has_connection() is runned .");
        return;
    }
    this->update_wifi_status_list();


    if(this->wifi_status_list.baterry < this->params_.wifi_batr_min)
    {
        Logger::log_debug("if(this->wifi_status_list.baterry < this->params_.wifi_batr_min)");

        this->wifi_device_charging_ = true;

        digitalWrite(this->params_.wifi_charge_pin, RELAY_ON);

    }
    else if (this->wifi_status_list.baterry >= this->params_.wifi_batr_max) {
        this->wifi_device_charging_ = false;
        digitalWrite(this->params_.wifi_charge_pin, RELAY_OFF);
    }
}

void ConfigJobsManager::update_wifi_status_list()
{
    WiFiClient client;
    HTTPClient http;

    const char* URL = "http://m.home/mark_conn.w.xml";

    

    Logger::log_debug("Sending HTTP request to m.home...");

    if (!http.begin(client, URL))
    {
        Logger::log_debug("HTTP begin() failed.");
        return;
    }

    Logger::log_debug(("WiFi status before HTTP: " + String(WiFi.status())).c_str());
    Logger::log_debug(("Connecting to: " + String(URL)).c_str());


    int httpCode = http.GET();
    if (httpCode > 0)
    {
        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            Logger::log_debug(("Server response: " + payload).c_str());

            // --- Парсим <batt_p> ---
            String tagStart = "<batt_p>";
            String tagEnd   = "</batt_p>";
            int idx1 = payload.indexOf(tagStart);
            int idx2 = payload.indexOf(tagEnd);

            if (idx1 != -1 && idx2 != -1 && idx2 > idx1)
            {
                String batt_p = payload.substring(idx1 + tagStart.length(), idx2);
                batt_p.trim();

                this->wifi_status_list.baterry = batt_p.toInt();

                Logger::log_debug(("Battery percent = " + batt_p).c_str());

                // Пример логики — обновить флаг зарядки
                int batt_percent = batt_p.toInt();
                this->wifi_device_charging_ = (batt_percent > 95); // например, считаем зарядкой, если >95%
            }
            else
            {
                Logger::log_debug("Tag <batt_p> not found in response.");
            }
        }
        else
        {
            Logger::log_debug(("HTTP error code: " + String(httpCode)).c_str());
        }
    }
    else
    {
        Logger::log_debug(("HTTP request failed: " + http.errorToString(httpCode)).c_str());
    }

    http.end();
}

ConfigsJobsManagerBase* get_config_job_manager(const ConfigsJobsManagerBase::Params& params)
{
    Logger::log_debug("get_config_job_manager() is runned .");
    ConfigsJobsManagerBase* configs_job_manager =  new ConfigJobsManager(params);
    return configs_job_manager;
}

