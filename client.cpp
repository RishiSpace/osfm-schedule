#include "client.h"
#include "network_utils.h"
#include <thread>
#include <iostream>
#include <unistd.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Client::Client(const std::string& serverAddr, int regPort)
    : serverAddr_(serverAddr), regPort_(regPort) {}

void Client::run() {
    registerWithServer();
    listenForTasks();
}

void Client::registerWithServer() {
    json j;
    j["id"] = std::to_string(getpid());
    j["address"] = serverAddr_;
    j["cores"] = std::thread::hardware_concurrency();
    UDPSender sender(serverAddr_, regPort_);
    sender.send(j.dump());
}

void Client::listenForTasks() {
    UDPListener listener(9001);
    while (true) {
        std::string data = listener.receive();
        Task t = parseTask(data);
        std::thread(&Client::executeTask, this, t).detach();
    }
}

void Client::executeTask(const Task& t) {
    std::string cmd = t.command;
    system(cmd.c_str());
    // write done file
}

int main() {
    std::string serverAddr = "127.0.0.1"; // or your server's IP
    int regPort = 9000;                   // or your server's registration port
    Client client(serverAddr, regPort);
    client.run();
    return 0;
}