#include "server.h"
#include "network_utils.h"
#include <iostream>
#include <thread>

Server::Server(int port) : port_(port) {}

void Server::run() {
    std::thread listener(&Server::listenForClients, this);
    std::thread dispatcher(&Server::dispatchTasks, this);
    listener.join();
    dispatcher.join();
}

void Server::listenForClients() {
    UDPListener listener(port_);
    while (true) {
        auto msg = listener.receive();
        auto info = parseRegistration(msg);
        clients_[info.id] = info;
        std::cout << "Registered: " << info.id << " (" << info.cores << " cores)\n";
    }
}

void Server::dispatchTasks() {
    while (true) {
        if (!taskQueue_.empty()) {
            for (auto& [id, client] : clients_) {
                if (client.freeCores > 0) {
                    Task t = taskQueue_.back();
                    taskQueue_.pop_back();
                    sendTask(client.address, t);
                    client.freeCores--;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    // your server startup code
    return 0;
}