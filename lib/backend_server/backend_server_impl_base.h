#pragma once
#include "backend_server_params.h"
#include <string>

class ResponseBase
{
public:
    virtual std::string get_response_body() = 0;
    ResponseBase(std::string response_body);

protected:
    std::string response_body_;

};

class BackendServerImplBase
{
    public:

    public:
        // virtual void start() = 0;
         virtual void stop() {};
         virtual ResponseBase* send_request(const backend_server::RequestParams& request_params) = 0;
        // virtual bool isRunning() const = 0;
        virtual ~BackendServerImplBase() = default;
};