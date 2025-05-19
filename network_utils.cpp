#include "network_utils.h"
#include "client_info.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

using json = nlohmann::json;

UDPListener::UDPListener(int port) {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("UDPListener: socket");
        exit(1);
    }
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("UDPListener: bind");
        close(sockfd);
        exit(1);
    }
}

std::string UDPListener::receive() {
    char buf[4096];
    sockaddr_in src_addr;
    socklen_t addrlen = sizeof(src_addr);
    ssize_t n = recvfrom(sockfd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&src_addr, &addrlen);
    if (n < 0) {
        perror("UDPListener: recvfrom");
        return "";
    }
    buf[n] = '\0';
    return std::string(buf);
}

UDPSender::UDPSender(const std::string& addr, int port) {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("UDPSender: socket");
        exit(1);
    }
    std::memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, addr.c_str(), &dest_addr.sin_addr) <= 0) {
        perror("UDPSender: inet_pton");
        close(sockfd);
        exit(1);
    }
}

void UDPSender::send(const std::string& data) {
    ssize_t n = sendto(sockfd, data.c_str(), data.size(), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    if (n < 0) {
        perror("UDPSender: sendto");
    }
}

ClientInfo parseRegistration(const std::string& msg) {
    auto j = json::parse(msg);
    ClientInfo c;
    c.id = j["id"];
    c.address = j["address"];
    c.cores = j["cores"];
    c.freeCores = c.cores;
    return c;
}

Task parseTask(const std::string& msg) {
    auto j = json::parse(msg);
    Task t;
    t.id = j["id"];
    t.command = j["command"];
    t.inputs = j["inputs"].get<std::vector<std::string>>();
    t.outputs = j["outputs"].get<std::vector<std::string>>();
    return t;
}

void sendTask(const std::string& addr, const Task& t) {
    json j;
    j["id"] = t.id;
    j["command"] = t.command;
    j["inputs"] = t.inputs;
    j["outputs"] = t.outputs;
    UDPSender sender(addr, 9001);
    sender.send(j.dump());
}
