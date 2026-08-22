#include "server.h"

Server::Server(ServerConfig config) {
    std::cout << "test" << std::endl;
    this->mainconn = tools::networking::IPv4TCPServerSocket(config.ip.c_str(), config.port);
    std::cout << "test" << std::endl;
    this->mainconn.Listen();
    std::cout << "test" << std::endl;
}

void Server::Start() {
    std::cout << "test" << std::endl;
    this->serverthread = std::thread(ThreadedStart, this);
    std::cout << "test" << std::endl;
}

int Server::AcceptNew() {
    int client = this->mainconn.AcceptNew();
    std::cout << "client connected" << std::endl;
    return client;
}

const tools::networking::IPv4TCPServerSocket& Server::GetConn() {
    return this->mainconn;
}

const ServerConfig& Server::GetConfig() {
    return this->config;
}

const char* ServerNegotiate(tools::networking::IPv4TCPConnection *conn, const ServerConfig config) {
    negotation message;
    message.isencrypted = config.encrypted;
    message.autoswitch = config.autoswitch;
    message.version = 1;
    message.error = 0;
    conn->Send((const char*)&message, sizeof(message));
    RSAkeysend keyexchange;
    std::pair<tools::crypto::RSAPublicKey, tools::crypto::RSAPrivateKey> keypair = tools::crypto::RSAGenerateKeyPair(7919, 1009);
    tools::crypto::RSAPublicKey public = keypair.first;
    tools::crypto::RSAPrivateKey private = keypair.second;
    keyexchange.key = public;
    conn->Send((const char*)&keyexchange, sizeof(keyexchange));

}

void ThreadedStart(Server *server) {
    int fd = server->AcceptNew();
    tools::networking::IPv4TCPConnection connection(fd);
    Negotiate(&connection, server->GetConfig());
}