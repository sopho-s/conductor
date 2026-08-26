#include <stdint.h>
#include "./imports/tools/src/crypto.h"
#pragma once

struct negotiation {
    uint16_t version;
    uint8_t error;
    bool isencrypted;
    bool autoswitch;
};

struct RSAkeysend {
    tools::crypto::RSAPublicKey key;
};

struct AESkeysend {
    unsigned char key[16];
    uint64_t challenge;
};

struct giveid {
    uint64_t challengeresponse;
    unsigned char id[16];
};

struct giveident {
    unsigned int size;
    unsigned char id[16];
    unsigned char* ident;
};

struct login {
    unsigned char id[16];
    uint32_t otp;
};

struct giveinterfaceip {
    unsigned char id[16];
    uint32_t ip;
};

struct messages {
    unsigned int size;
    unsigned char id[16];
    unsigned char* message;
};