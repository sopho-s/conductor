#include "server.h"

Server::Server(ServerConfig config) {
    this->mainconn = tools::networking::IPv4TCPServerSocket(config.ip.c_str(), config.port);
    this->mainconn.Listen();
}

void Server::Start() {
    this->serverthread = std::thread(ThreadedStart, this);
}

int Server::AcceptNew() {
    int client = this->mainconn.AcceptNew();
    return client;
}

const tools::networking::IPv4TCPServerSocket& Server::GetConn() {
    return this->mainconn;
}

const ServerConfig& Server::GetConfig() {
    return this->config;
}

unsigned char* ServerNegotiate(tools::networking::IPv4TCPConnection *conn, const ServerConfig config) {
    negotiation negmessage;
    negmessage.isencrypted = config.encrypted;
    negmessage.autoswitch = config.autoswitch;
    negmessage.version = 1;
    negmessage.error = 0;
    conn->Send((const char*)&negmessage, sizeof(negmessage));
    RSAkeysend keyexchange;
    std::pair<tools::crypto::RSAPublicKey, tools::crypto::RSAPrivateKey> keypair = tools::crypto::RSAGenerateKeyPair(7919, 1009);
    tools::crypto::RSAPublicKey _public = keypair.first;
    tools::crypto::RSAPrivateKey _private = keypair.second;
    keyexchange.key = _public;
    conn->Send((const char*)&keyexchange, sizeof(keyexchange));
    unsigned char* response = conn->Receive(sizeof(uint32_t));
    uint32_t size;
    memcpy(&size, response, sizeof(uint32_t));
    delete[] response;
    response = conn->Receive(size);
    for (int i = 0; i < 16; i++) {
        tools::crypto::RSADecrypt<unsigned char>(response[i], _private);
    }
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
    unsigned char* message = new unsigned char[sizeof(giveid)];
    memcpy(message, &idmessage, sizeof(giveid));
    uint32_t encsize = tools::crypto::EncryptAES128(message, key, sizeof(giveid));
    conn->Send((const char*)&encsize, sizeof(uint32_t));
    conn->Send((const char*)message, encsize);
    tools::crypto::DecryptAES128(message, key, encsize);
    unsigned char ret[32];
    memcpy(ret, idmessage.id, 16);
    memcpy(ret + 16, aeskeysend.key, 16);
    return ret;
}

void ConnectionManager(tools::networking::IPv4TCPConnection *newconn, unsigned char* keyandid, const ServerConfig config) {
    unsigned char *id = keyandid;
    unsigned char *key = keyandid + 16;
    unsigned char *message;
    uint32_t size = Recieve(newconn, key, message);
    giveident ident;
    memcpy(&ident, message, sizeof(login));
    delete[] message;
    for (int i = 0; i < 16; i++) {
        if (ident.id[i] != id[i]) {
            newconn->Close();
            return;
        }
    }
    bool isbackend = (ident.size == config.backendident.size());
    for (int i = 0; i < ident.size; i++) {
        if (ident.ident[i] != config.backendident.c_str()[i]) {
            isbackend = false;
        }
    }
    bool ismusician = (ident.size == config.musicianident.size());
    for (int i = 0; i < ident.size; i++) {
        if (ident.ident[i] != config.musicianident.c_str()[i]) {
            ismusician = false;
        }
    }
    if (isbackend) {
    } else if (ismusician) {
        
    }
}

void ThreadedStart(Server *server) {
    while (true) {
        int fd = server->AcceptNew();
        std::cout << "client connected" << std::endl;
        tools::networking::IPv4TCPConnection *connection = new tools::networking::IPv4TCPConnection(fd);
        unsigned char* keyandid = ServerNegotiate(connection, server->GetConfig());
        std::cout << "encryption keys negotiated" << std::endl;
        std::thread thread = std::thread(ConnectionManager, connection, keyandid, server->GetConfig());
    }
}