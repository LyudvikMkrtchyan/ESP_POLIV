#pragma once

#include <backend_server_impl_base.h>
#include "backend_server_params.h"

class HttpServer: public BackendServerImplBase
{
    public: 
        HttpServer(const backend_server::BackendServerParams& params);
        void send_request(const backend_server::RequestParams& request_params) override;
        // void start() override;
        // void stop() override;
        // bool isRunning() const override;

    private:
    
    private:
        backend_server::BackendServerParams params_;
        
};