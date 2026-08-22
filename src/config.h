#include <stdint.h>
#include <string>
#pragma once

struct ServerConfig {
    std::string ip;
    uint16_t port;
    std::string backendident;
    bool encrypted;
    bool autoswitch;
};

struct ClientConfig {
    std::string ip;
    uint16_t port;
    std::string ident;
};