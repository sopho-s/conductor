#include "./imports/tools/src/networking/tcp.h"
#include "./imports/tinyxml2/tinyxml2.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc > 2) {
        std::string configfilename = std::string(argv[2]);
        tools::io::File *configfile = static_cast<tools::io::File*>(tools::io::ReadFile(configfilename));
        tinyxml2::XMLDocument doc;
        doc.Parse(configfile->contents.c_str());
        if (strcmp(argv[1], "server") == 0) {
            auto serversock = tools::networking::IPv4TCPServerSocket("127.0.0.1", 8080);
            serversock.Listen();
            (void)serversock.AcceptNew();
            std::cout << "client connected" << std::endl;
        } else if (strcmp(argv[1], "client") == 0) {
            auto sock = tools::networking::IPv4TCPClientSocket("127.0.0.1", 8080);
            sock.Send("test", 4);
            std::cout << "connected to server" << std::endl;
        }
        delete configfile;
    } else {
        throw std::runtime_error("conductor (server|client) (config file as xml)");
    }
}