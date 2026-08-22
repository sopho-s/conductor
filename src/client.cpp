#include "client.h"

Client::Client(ClientConfig config) {
    this->config = config;
    this->mainconn = tools::networking::IPv4TCPClientSocket(config.ip.c_str(), config.port);
}

void Client::Send(const char *message, int size) {
    this->mainconn.Send(message, size);
}