#include "server.h"
#include "network_utils.h"
#include <iostream>
#include <thread>

Server::Server(int port) : port_(port) {}

void Server::run() {
    std::cout << "[SERVER] Running main server loop..." << std::endl;
    std::thread listener(&Server::listenForClients, this);
    std::thread dispatcher(&Server::dispatchTasks, this);
    std::thread inputThread(&Server::readTasksFromStdin, this);
    listener.join();
    dispatcher.join();
    inputThread.join();
}

void Server::listenForClients() {
    std::cout << "[SERVER] Listening for client registrations on port " << port_ << "..." << std::endl;
    UDPListener listener(port_);
    while (true) {
        auto msg = listener.receive();
        std::cout << "[SERVER] Received registration message: " << msg << std::endl;
        auto info = parseRegistration(msg);
        clients_[info.id] = info;
        std::cout << "[SERVER] Registered: " << info.id << " (" << info.cores << " cores)" << std::endl;
    }
}

void Server::dispatchTasks() {
    std::cout << "[SERVER] Dispatching tasks to clients..." << std::endl;
    while (true) {
        if (!taskQueue_.empty()) {
            for (auto& [id, client] : clients_) {
                if (client.freeCores > 0) {
                    Task t = taskQueue_.back();
                    taskQueue_.pop_back();
                    std::cout << "[SERVER] Sending task " << t.id << " to client " << client.id << std::endl;
                    sendTask(client.address, t);
                    client.freeCores--;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Server::readTasksFromStdin() {
    std::cout << "[SERVER] You can enter commands to dispatch to clients. Type 'exit' to quit." << std::endl;
    int taskId = 1;
    std::string line;
    while (true) {
        std::cout << "[SERVER] Enter command: ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit") break;
        if (line.empty()) continue;
        Task t;
        t.id = std::to_string(taskId++);
        t.command = line;
        t.inputs = {};
        t.outputs = {};
        taskQueue_.push_back(t);
        std::cout << "[SERVER] Task queued: " << t.command << std::endl;
    }
    std::cout << "[SERVER] Stdin task input thread exiting." << std::endl;
}

int main() {
    std::cout << "[SERVER] Starting server on port 9000..." << std::endl;
    Server server(9000);
    server.run();
    return 0;
}