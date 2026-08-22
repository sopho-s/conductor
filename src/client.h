#include "config.h"
#include "./imports/tools/src/networking/tcp.h"
#include <thread>
#pragma once

struct Client {
    private:
        ClientConfig config;
        tools::networking::IPv4TCPClientSocket mainconn;
    public:
        Client() {}
        ~Client() {}
        Client(ClientConfig config);
        void Send(const char* message, int size);
};