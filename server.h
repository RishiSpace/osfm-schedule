#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <string>
#include <map>
#include "client_info.h"
#include "task.h"

class Server {
public:
    Server(int port);
    void run();

private:
    void listenForClients();
    void handleClientRegistration();
    void dispatchTasks();

    int port_;
    std::map<std::string, ClientInfo> clients_; // key: client ID
    std::vector<Task> taskQueue_;
};

#endif