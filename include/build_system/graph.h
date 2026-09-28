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
        void add_target(const Target* new_target);
        void add_edge(const std::string main, const std::string dependent);
        bool valid_build();
};

