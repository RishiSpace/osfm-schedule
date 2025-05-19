#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include "task.h"

class Client {
public:
    Client(const std::string& serverAddr, int regPort);
    void run();

private:
    void registerWithServer();
    void listenForTasks();
    void executeTask(const Task& t);

    std::string serverAddr_;
    int regPort_;
    std::vector<Task> runningTasks_;
};

#endif