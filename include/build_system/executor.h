#pragma once

#include <string>
#include <filesystem>

#include "build_system/graph.h"
#include "build_system/target.h"

class Executor {
    private:
        Graph& g;
        const bool keep_going;
        std::string build_dir;

    public:
        bool run();
        Executor(Graph& g, const bool keep_going, const std::string& build_file_path)
            : g{g}, keep_going{keep_going},
              build_dir{std::filesystem::path(build_file_path).parent_path().string()} {};
};