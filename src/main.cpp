#include <iostream>

#include "build_system/graph.h"
#include "build_system/parser.h"
#include "build_system/executor.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <build_file> [-k]\n";
        return 1;
    }

    std::string filename = argv[1];
    bool keep_going = false;

    for (int i = 2; i < argc; i++) {
        if (std::string(argv[i]) == "-k") {
            keep_going = true;
        }
    }

    try {
        Graph g;
        Parser parser(filename, g);
        parser.parseFile();

        Executor executor(g, keep_going, filename);
        bool success = executor.run();

        if (!success) {
            std::cerr << "build finished with failures\n";
            return 1;
        }

        std::cout << "build succeeded\n";
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "build failed: " << e.what() << "\n";
        return 1;
    }
}