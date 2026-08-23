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
    tools::crypto::RSAPublicKey _public = keypair.first;
    tools::crypto::RSAPrivateKey _private = keypair.second;
    keyexchange.key = _public;
    conn->Send((const char*)&keyexchange, sizeof(keyexchange));
    unsigned char* response = conn->Receive(sizeof(AESkeysend));
    tools::crypto::RSADecrypt<unsigned char>(*response, _private);
    AESkeysend aeskeysend;
    memcpy(&aeskeysend, response, sizeof(AESkeysend));
    unsigned char key[16];
    memcpy(key, aeskeysend.key, 16);
    giveid idmessage;
    idmessage.challengeresponse = aeskeysend.challenge;
    for (int i = 0; i < 4; i++) {
        uint32_t tmp = rand();
        memcpy(idmessage.id + i * 4, &tmp, sizeof(uint32_t));
    }
    conn->Send((const char*)&idmessage, sizeof(giveid));
    unsigned char ret[32];
    memcpy(ret, idmessage.id, 16);
    memcpy(ret + 16, aeskeysend.key, 16);
    return nullptr;
}

void ConnectionManager(tools::networking::IPv4TCPConnection *newconn, const char* keyandid) {
    
}

void ThreadedStart(Server *server) {
    while (true) {
        int fd = server->AcceptNew();
        tools::networking::IPv4TCPConnection *connection = new tools::networking::IPv4TCPConnection(fd);
        const char* keyandid = ServerNegotiate(connection, server->GetConfig());
        std::thread thread = std::thread(ConnectionManager, connection, keyandid);
    }
}