#include "client.h"

Client::Client(ClientConfig config) {
    this->config = config;
    std::cout << "connecting to server" << std::endl;
    this->mainconn = tools::networking::IPv4TCPClientSocket(config.ip.c_str(), config.port);
    std::cout << "connected to server" << std::endl;
    if (!this->ClientNegotiate()) {
        throw std::runtime_error("challenge did not match response");
    }
    std::cout << "encryption keys negotiated" << std::endl;
}

void Client::Send(const char *message, int size) {
    this->mainconn.Send(message, size);
}

bool Client::ClientNegotiate() {
    negotiation serverstuff;
    unsigned char *message = this->mainconn.Receive(sizeof(negotiation));
    memcpy(&serverstuff, message, sizeof(negotiation));
    delete[] message;
    this->serverconfig.autoswitch = serverstuff.autoswitch;
    this->serverconfig.encrypted = serverstuff.isencrypted;
    RSAkeysend keycontainer;
    message = this->mainconn.Receive(sizeof(RSAkeysend));
    memcpy(&keycontainer, message, sizeof(RSAkeysend));
    delete[] message;
    tools::crypto::RSAPublicKey key = keycontainer.key;
    AESkeysend keysend;
    // REMOVE WITH RANDOM KEY GEN
    unsigned char tmpkey[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    memcpy(keysend.key, tmpkey, 16);
    memcpy(this->key, tmpkey, 16);
    keysend.challenge = rand_uint64();
    for (int i = 0; i < sizeof(keysend); i++) {
        unsigned char *tmppointer = ((unsigned char*)&keysend) + i;
        tools::crypto::RSAEncrypt(*tmppointer, key);
    }
    uint32_t messagesize = sizeof(keysend);
    this->mainconn.Send((const char*)&messagesize, sizeof(uint32_t));
    this->mainconn.Send((const char*)&keysend, sizeof(AESkeysend));
    message = this->mainconn.Receive(sizeof(uint32_t));
    memcpy(&messagesize, message, sizeof(uint32_t));
    delete[] message;
    message = this->mainconn.Receive(messagesize);
    tools::crypto::DecryptAES128(message, this->key, messagesize);
    giveid idstuff;
    memcpy(&idstuff, message, sizeof(giveid));
    delete[] message;
    if (keysend.challenge != idstuff.challengeresponse) {
        return false;
    }
    memcpy(this->id, idstuff.id, 16);
    return true;
}