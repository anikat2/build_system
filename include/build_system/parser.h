#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "build_system/graph.h"
#include "build_system/target.h"

class Parser {
    private:
        std::string filename;
        Graph& g;

        void addEdges();
    public:
        Parser(const std::string& filename, Graph& g) : filename{filename}, g{g} {};
        void parseFile();
};