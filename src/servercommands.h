struct addalink {
    uint16_t nonce;
    uint8_t commandnumber = 0;
    uint32_t identsize;
    const char *userident;
    uint64_t linkid;
}

struct addalinkresponse {
    uint16_t nonce;
    uint8_t iserror;
    uint32_t ipinterface;
}

struct removealink {
    uint16_t nonce;
    uint8_t commandnumber = 1;
    uint32_t identsize;
    const char *userident;
    uint64_t linkid;
}

struct removealink {
    uint16_t nonce;
    uint8_t iserror;
}

struct addacontainer {
    uint16_t nonce;
    uint8_t commandnumber = 2;
    uint32_t identsize;
    const char *userident;
    uint64_t linkid;
    uint64_t containerid;
}

struct addacontainerresponse {
    uint16_t nonce;
    uint8_t iserror;
}

struct linkacontainer {
    uint16_t nonce;
    uint8_t commandnumber = 2;
    uint32_t identsize;
    const char *userident;
    uint64_t linkid;
    uint64_t linkid2;
}

struct linkacontainerresponse {
    uint16_t nonce;
    uint8_t iserror;
    uint64_t linkid;
}