#include "encryptednetworking.h"
void Send(tools::networking::IPv4TCPConnection *newconn, unsigned char* message, unsigned char* key, uint32_t size) {
    uint32_t encsize = tools::crypto::EncryptAES128(message, key, size);
    newconn->Send((const char*)&encsize, sizeof(uint32_t));
    newconn->Send((const char*)message, encsize);
}

int Recieve(tools::networking::IPv4TCPConnection *newconn, unsigned char* key, unsigned char* &out) {
    uint32_t size;
    unsigned char *message = newconn->Receive(sizeof(uint32_t));
    memcpy(&size, message, sizeof(uint32_t));
    out = newconn->Receive(size);
    tools::crypto::DecryptAES128(out, key, size);
    return size;
}

int Recieve(tools::networking::IPv4TCPConnection *newconn, unsigned char* key, messages &out) {
    uint32_t size;
    unsigned char *message = newconn->Receive(sizeof(uint32_t));
    memcpy(&size, message, sizeof(uint32_t));
    newconn->Receive(size);
    tools::crypto::DecryptAES128(message, key, size);
    memcpy(&out.id, message, 16);
    memcpy(&out.size, message + 16, sizeof(uint32_t));
    out.message = new unsigned char[out.size];
    memcpy(out.message, message + 16 + sizeof(uint32_t), out.size);
    return size;
}


void Send(tools::networking::IPv4TCPConnection *newconn, messages &message, unsigned char* key) {
    unsigned char* charmessage;
    charmessage = new unsigned char[sizeof(unsigned int) + 16 + message.size];
    memcpy(charmessage, &message, sizeof(unsigned int) + 16);
    memcpy(charmessage + sizeof(unsigned int) + 16, message.message, message.size);
    uint32_t encsize = tools::crypto::EncryptAES128(charmessage, key, (int)(sizeof(unsigned int) + 16 + message.size));
    newconn->Send((const char*)&encsize, sizeof(uint32_t));
    newconn->Send((const char*)&message, encsize);
}