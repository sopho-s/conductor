#include "config.h"
#include "messagestructs.h"
#include "./imports/tools/src/networking/tcp.h"
#include "./imports/tools/src/crypto.h"
#include <thread>
#pragma once

struct Client {
    private:
        ServerConfig serverconfig;
        ClientConfig config;
        tools::networking::IPv4TCPClientSocket mainconn;
        unsigned char id[16];
        unsigned char key[16];
    public:
        Client() {}
        ~Client() {}
        Client(ClientConfig config);
        void Send(const char* message, int size);
        bool ClientNegotiate();
};