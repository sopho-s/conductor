#include <stdint.h>
#include "./imports/tools/src/crypto.h"
#pragma once

struct negotation {
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

struct login {
    unsigned char id[16];
    uint32_t otp;
};

struct giveinterfaceip {
    unsigned char id[16];
    uint32_t ip;
};

struct messages {
    unsigned char id[16];
    uint32_t size;
    unsigned char* message;
};