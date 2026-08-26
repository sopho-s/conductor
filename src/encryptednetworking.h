#include "./imports/tools/src/networking/tcp.h"
#include "./imports/tools/src/crypto.h"
#include "messagestructs.h"
#pragma once

void Send(tools::networking::IPv4TCPConnection *newconn, unsigned char* message, unsigned char* key, uint32_t size);
int Recieve(tools::networking::IPv4TCPConnection *newconn, unsigned char* key, unsigned char* &out);
int Recieve(tools::networking::IPv4TCPConnection *newconn, unsigned char* key, messages &out);
void Send(tools::networking::IPv4TCPConnection *newconn, messages &message, unsigned char* key);