#ifndef CLIENT_INFO_H
#define CLIENT_INFO_H

#include <string>

struct ClientInfo {
    std::string id;
    std::string address;
    int cores;
    int freeCores;
};

#endif