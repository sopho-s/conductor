#include <stdint.h>
#include <string>
#pragma once

struct ServerConfig {
    std::string ip;
    uint16_t port;
    std::string backendident;
    std::string musicianident;
    std::string bridgeident;
    bool encrypted;
    bool autoswitch;
};

struct ClientConfig {
    std::string ip;
    uint16_t port;
    std::string ident;
};