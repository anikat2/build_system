#include <gtest/gtest.h>
#include <fstream>

#include "build_system/graph.h"
#include "build_system/executor.h"

static std::string write_temp_build_file(const std::string& contents, const std::string& name) {
    std::string path = name;
    std::ofstream out(path);
    out << contents;
    out.close();
    return path;
}

TEST(ExecutorTest, SuccessfulBuildReturnsTrue) {
    Graph g;
    g.add_target(Target("a.txt", {}, "echo hello > a.txt"));
    g.add_target(Target("b.txt", {"a.txt"}, "echo world > b.txt"));
    g.add_edge("a.txt", "b.txt");

    std::string fake_path = "./executor_test_dummy.txt";

    Executor executor(g, /*keep_going=*/false, fake_path);
    bool result = executor.run();

    EXPECT_TRUE(result);

    std::remove("a.txt");
    std::remove("b.txt");
}

TEST(ExecutorTest, FailingCommandThrowsWhenNotKeepGoing) {
    Graph g;
    g.add_target(Target("bad.txt", {}, "this_command_does_not_exist_12345"));

    std::string fake_path = "./executor_test_dummy.txt";

    Executor executor(g, /*keep_going=*/false, fake_path);

    EXPECT_THROW(executor.run(), std::runtime_error);
}

TEST(ExecutorTest, FailingCommandReturnsFalseWhenKeepGoing) {
    Graph g;
    g.add_target(Target("bad.txt", {}, "this_command_does_not_exist_12345"));
    g.add_target(Target("good.txt", {}, "echo ok > good.txt"));

    std::string fake_path = "./executor_test_dummy.txt";

    Executor executor(g, /*keep_going=*/true, fake_path);
    bool result = executor.run();

    EXPECT_FALSE(result);

    std::remove("good.txt");
}

TEST(ExecutorTest, CyclicGraphThrows) {
    Graph g;
    g.add_target(Target("a.txt", {}, "echo a > a.txt"));
    g.add_target(Target("b.txt", {}, "echo b > b.txt"));
    g.add_edge("a.txt", "b.txt");
    g.add_edge("b.txt", "a.txt");

    std::string fake_path = "./executor_test_dummy.txt";

    Executor executor(g, /*keep_going=*/false, fake_path);

    EXPECT_THROW(executor.run(), std::runtime_error);
}