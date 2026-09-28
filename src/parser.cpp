#include <stdexcept>
#include <sstream>

#include "build_system/parser.h"
#include "build_system/target.h"

/*
std::string filename;
Graph& g;
*/

void Parser::addEdges() {
    std::unordered_map<std::string, Target> targets = g.getTargets();

    for (const auto& [name, target] : targets) {
        for (const std::string& input : target.inputs) {
            if (targets.count(input) > 0) {
                g.add_edge(input, name);
            }
        }
    }
}

void Parser::parseFile() {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("file cannot be opened -> check perms");
    }

    std::string line;

    std::string name;
    std::vector<std::string> inputs;
    std::string command;
    bool has_data = false;

    auto flush_block = [&]() {
        if (has_data) {
            g.add_target(Target(name, inputs, command));
            name.clear();
            inputs.clear();
            command.clear();
            has_data = false;
        }
    };

    while (std::getline(file, line)) {
        if (line.empty()) {
            flush_block();
            continue;
        }

        std::stringstream ss(line);
        std::string key;
        std::getline(ss, key, ':');

        std::string value;
        std::getline(ss, value);

        if (!value.empty() && value[0] == ' ') {
            value = value.substr(1);
        }

        if (key == "target") {
            name = value;
            has_data = true;
        } else if (key == "inputs") {
            std::stringstream vs(value);
            std::string token;
            while (vs >> token) {
                inputs.push_back(token);
            }
        } else if (key == "command") {
            command = value;
        }
    }

    flush_block();

    file.close();

    addEdges();
}