
#pragma once

#include "backend_server_impl_base.h"
#include "backend_server_params.h"
#include "readers_base.hpp"
#include <string>
#include <vector>


class BackendServers{
    public:
        static BackendServers& getInstance();
        static void init(const atl::File& backend_server_config_file);

        void init_internal(const atl::File& backend_server_config_file);
        ResponseBase* send_request(const backend_server::RequestParams& request_params);


    private:
    backend_server::BackendConfig parse_config_file(const atl::File& backend_server_config_file);

    private:
        bool running_ = false;
        
        std::vector<BackendServerImplBase*> servers_;
        int curent_server_index_ = 0;

        std::string output_file_name_;
};