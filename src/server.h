#include "config.h"
#include "messagestructs.h"
#include "encryptednetworking.h"
#include "./imports/tools/src/networking/tcp.h"
#include <random>
#include <thread>
#pragma once

struct Server {
    private:
        ServerConfig config;
        std::thread serverthread;
        tools::networking::IPv4TCPServerSocket mainconn;
    public:
        Server() {}
        ~Server() {}
        Server(ServerConfig config);
        void Start();
        int AcceptNew();
        const tools::networking::IPv4TCPServerSocket& GetConn();
        const ServerConfig& GetConfig();
};

unsigned char*  ServerNegotiate(Server *server);

void ConnectionManager(tools::networking::IPv4TCPConnection *newconn, unsigned char* keyandid, const ServerConfig config);
void ThreadedStart(Server *server);