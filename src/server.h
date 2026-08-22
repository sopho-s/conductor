#include "config.h"
#include "messagestructs.h"
#include "./imports/tools/src/networking/tcp.h"
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

void ServerNegotiate(Server *server);

void ThreadedStart(Server *server);