#include <gtest/gtest.h>
#include <fstream>
#include <algorithm>

#include "build_system/parser.h"
#include "build_system/graph.h"
#include "test_helpers.h"

std::string write_temp_file(const std::string& contents, const std::string& name) {
    std::string path = name;
    std::ofstream out(path);
    out << contents;
    out.close();
    return path;
}

TEST(ParserTest, ValidFileProducesCorrectGraph) {
    std::string build_file_contents =
        "target: main.o\n"
        "inputs: main.cpp utils.h\n"
        "command: g++ -c main.cpp -o main.o\n"
        "\n"
        "target: program\n"
        "inputs: main.o utils.o\n"
        "command: g++ main.o utils.o -o program\n";

    std::string path = write_temp_file(build_file_contents, "test_valid_build.txt");

    Graph g;
    Parser parser(path, g);
    parser.parseFile();

    auto [valid, order] = g.valid_build();

    EXPECT_TRUE(valid);
    EXPECT_EQ(order.size(), 2);
    EXPECT_TRUE(appears_before(order, "main.o", "program"));

    std::remove(path.c_str());
}

TEST(ParserTest, TargetFieldsAreParsedCorrectly) {
    std::string build_file_contents =
        "target: main.o\n"
        "inputs: main.cpp utils.h\n"
        "command: g++ -c main.cpp -o main.o\n";

    std::string path = write_temp_file(build_file_contents, "test_fields_build.txt");

    Graph g;
    Parser parser(path, g);
    parser.parseFile();

    const auto& targets = g.getTargets();
    ASSERT_EQ(targets.count("main.o"), 1);

    const Target& t = targets.at("main.o");
    EXPECT_EQ(t.name, "main.o");
    EXPECT_EQ(t.command, "g++ -c main.cpp -o main.o");
    ASSERT_EQ(t.inputs.size(), 2);
    EXPECT_EQ(t.inputs[0], "main.cpp");
    EXPECT_EQ(t.inputs[1], "utils.h");

    std::remove(path.c_str());
}

TEST(ParserTest, NoTrailingBlankLineStillParsesLastBlock) {
    std::string build_file_contents =
        "target: only.o\n"
        "inputs: only.cpp\n"
        "command: g++ -c only.cpp -o only.o";

    std::string path = write_temp_file(build_file_contents, "test_no_trailing_blank.txt");

    Graph g;
    Parser parser(path, g);
    parser.parseFile();

    auto [valid, order] = g.valid_build();

    EXPECT_TRUE(valid);
    EXPECT_EQ(order.size(), 1);

    std::remove(path.c_str());
}

TEST(ParserTest, NonexistentFileThrows) {
    Graph g;
    Parser parser("this_file_does_not_exist.txt", g);

    EXPECT_THROW(parser.parseFile(), std::runtime_error);
}