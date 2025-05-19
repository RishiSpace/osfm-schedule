#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <string>
#include <netinet/in.h> // Add this for sockaddr_in
#include "task.h"
#include "client_info.h" // <-- Add this line

struct UDPListener {
    int sockfd; // <-- Add this
    UDPListener(int port);
    std::string receive();
};

struct UDPSender {
    int sockfd; // <-- Add this
    struct sockaddr_in dest_addr; // <-- Add this
    UDPSender(const std::string& addr, int port);
    void send(const std::string& data);
};

ClientInfo parseRegistration(const std::string& msg);
void sendTask(const std::string& addr, const Task& t);
Task parseTask(const std::string& msg);

#endif