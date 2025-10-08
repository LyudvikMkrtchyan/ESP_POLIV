#include "configs_job_manager_base.hpp"
#include "job_params.h"
#include "time_base.hpp"
#include "list"
#include <map>


class ConfigJobsManager: public ConfigsJobsManagerBase
{
    public:
        void run_configs_loop() override;
        ConfigJobsManager(const ConfigsJobsManagerBase::Params& params):ConfigsJobsManagerBase(params){}

    private:

        struct WifiStatusList
        {
            int baterry = 0;
        };

        void monitor_wifi_battery_status();
        void update_wifi_status_list();

        
        bool wifi_device_charging_ = false ;
        WifiStatusList wifi_status_list;
};