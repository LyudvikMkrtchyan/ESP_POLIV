#pragma once
#include "backend_server_params.h"

class BackendServerImplBase
{
    public:

    public:
        // virtual void start() = 0;
         virtual void stop() {};
         virtual void send_request(const backend_server::RequestParams& request_params) = 0;
        // virtual bool isRunning() const = 0;
        virtual ~BackendServerImplBase() = default;
};