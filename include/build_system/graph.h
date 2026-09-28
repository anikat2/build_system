#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "target.h"

class Graph {
    private:
        std::unordered_map<std::string, Target> targets;
        std::unordered_map<std::string, std::vector<Target*>> adj_map;
        std::unordered_map<std::string, int> indegree_counts;
    public:
        void add_target(const Target& new_target);
        const std::unordered_map<std::string, Target>& getTargets();
        void add_edge(const std::string& dependency, const std::string& dependent);
        std::pair<bool, std::vector<std::string>> valid_build();
};

