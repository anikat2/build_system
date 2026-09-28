#include <stdexcept>
#include <queue>

#include "build_system/graph.h"

/* 
std::unordered_map<std::string, Target> targets;
std::unordered_map<std::string, std::vector<Target*>> adj_map;
std::unordered_map<std::string, int> indegree_counts;
*/

const std::unordered_map<std::string, Target>& Graph::getTargets() {
    return targets;
}

void Graph::add_target(const Target& new_target) {
    targets.emplace(new_target.name, new_target);
    indegree_counts[new_target.name];
}

void Graph::add_edge(const std::string& dependency, const std::string& dependent) {
    if (targets.count(dependency) == 0 || targets.count(dependent) == 0) {
        throw std::runtime_error("unknown dependencies -> double check input");
    }
    Target* d2 = &targets[dependent];

    adj_map[dependency].push_back(d2);
    indegree_counts[dependent]++;
}

std::pair<bool, std::vector<std::string>> Graph::valid_build() {
    std::queue<std::string> q;
    std::vector<std::string> result;

    for (const auto& [name, count] : indegree_counts) {
        if (count == 0) {
            q.push(name);
        }
    }

    while (!q.empty()) {
        std::string curr = q.front();
        q.pop();

        result.push_back(curr);

        const std::vector<Target*>& neighbors = adj_map[curr];

        for (Target* neighbor : neighbors) {
            if (--indegree_counts[neighbor -> name] == 0) {
                q.push(neighbor -> name);
            }
        }
    }

    return {targets.size() == result.size(), result};
}