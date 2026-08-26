#include "./imports/tools/src/networking/tcp.h"
#include "./imports/tools/src/io.h"
#include "./imports/tinyxml2/tinyxml2.h"
#include "./config.h"
#include "server.h"
#include "client.h"
#include <iostream>


int main(int argc, char *argv[]) {
    if (argc > 2) {
        std::string configfilename = std::string(argv[2]);
        auto fileobj = tools::io::ReadFile(configfilename);
        tools::io::GetFileContents(fileobj);
        tools::io::File *configfile = static_cast<tools::io::File*>(fileobj);
        tinyxml2::XMLDocument doc;
        doc.Parse(configfile->contents.c_str());
        if (strcmp(argv[1], "server") == 0) {
            ServerConfig config;
            auto addr = doc.FirstChildElement("root")->FirstChildElement("addr");
            config.ip = std::string(addr->FirstChildElement("ip")->GetText());
            config.port = std::stoul(addr->FirstChildElement("port")->GetText());
            config.backendident = std::string(doc.FirstChildElement("root")->FirstChildElement("backendident")->GetText());
            config.musicianident = std::string(doc.FirstChildElement("root")->FirstChildElement("musicianident")->GetText());
            config.bridgeident = std::string(doc.FirstChildElement("root")->FirstChildElement("bridgeident")->GetText());
            config.encrypted = (std::string(doc.FirstChildElement("root")->FirstChildElement("encrypted")->GetText()) == "true");
            config.autoswitch = (std::string(doc.FirstChildElement("root")->FirstChildElement("autoswitch")->GetText()) == "true");
            Server server = Server(config);
            server.Start();
            while (true) {
                ;
            }
        } else if (strcmp(argv[1], "client") == 0) {
            ClientConfig config;
            auto addr = doc.FirstChildElement("root")->FirstChildElement("addr");
            config.ip = std::string(addr->FirstChildElement("ip")->GetText());
            config.port = std::stoul(addr->FirstChildElement("port")->GetText());
            config.ident = std::string(doc.FirstChildElement("root")->FirstChildElement("ident")->GetText());
            Client client = Client(config);
        }
        delete configfile;
    } else {
        throw std::runtime_error("conductor (server|client) (config file as xml)");
    }
}