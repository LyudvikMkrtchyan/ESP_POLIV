#pragma once

#include <Arduino.h> 
#include <vector>

namespace backend_server{

enum class RequestType {
    GET,
    POST,
    PUT,
    DELETE,
    UNKNOWN
};

struct RequestParams {
    RequestType type;
    String method;
    String endpoint;
    String body;
};



  struct BackendServerParams {
    int port;
    String ip_address;
};

struct BackendServerImplParams {
    String type;
    BackendServerParams params;
};

struct BackendConfig {
    String output_file_name;
    std::vector<BackendServerImplParams*> servers;
};



}