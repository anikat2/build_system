#include <cstdlib>
#include <stdexcept>

#include "build_system/executor.h"

/* 
Graph& g;
const bool keep_going;
std::string build_dir;
*/

bool Executor::run() {
    auto [valid, order] = g.valid_build();

    if (!valid) {
        throw std::runtime_error("cyclic dependencies");
    }

    const auto& targets = g.getTargets();
    bool all_succeeded = true;

    for (const auto& name : order) {
        std::string full_command = build_dir.empty() ? targets.at(name).command : "cd /d \"" + build_dir + "\" && " + targets.at(name).command;
        int exit_code = std::system(full_command.c_str());

        if (exit_code != 0) {
            all_succeeded = false;

            if (!keep_going) {
                throw std::runtime_error("build failed on target: " + name);
            }
        }
    }

    return all_succeeded;
}