#pragma once

#include <string>
#include <vector>

class Target {
    public:
        std::string name;
        std::vector<std::string> inputs;
        std::string command;

        Target (std::string name, std::vector<std::string> inputs, std::string command) : name{name}, inputs{inputs}, command{command} {};
};