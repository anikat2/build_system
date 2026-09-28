#pragma once

#include <string>
#include <vector>

class Target {
    public:
        std::string name;
        std::vector<std::string> inputs;
        std::string command;
};