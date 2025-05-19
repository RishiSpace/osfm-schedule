#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>

struct Task {
    std::string id;
    std::string command;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
};

#endif