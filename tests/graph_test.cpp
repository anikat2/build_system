#include <gtest/gtest.h>
#include "build_system/graph.h"
#include "test_helpers.h"

TEST(GraphTest, LinearChainIsValid) {
    Graph g;
    g.add_target({"A", {}, "cmd_a"});
    g.add_target({"B", {}, "cmd_b"});
    g.add_target({"C", {}, "cmd_c"});

    g.add_edge("A", "B"); // B depends on A
    g.add_edge("B", "C"); // C depends on B

    auto [valid, order] = g.valid_build();

    EXPECT_TRUE(valid);
    EXPECT_EQ(order.size(), 3);
    EXPECT_TRUE(appears_before(order, "A", "B"));
    EXPECT_TRUE(appears_before(order, "B", "C"));
}

TEST(GraphTest, DiamondIsValid) {
    Graph g;
    g.add_target({"A", {}, "cmd_a"});
    g.add_target({"B", {}, "cmd_b"});
    g.add_target({"C", {}, "cmd_c"});
    g.add_target({"D", {}, "cmd_d"});

    g.add_edge("A", "B"); // B depends on A
    g.add_edge("A", "C"); // C depends on A
    g.add_edge("B", "D"); // D depends on B
    g.add_edge("C", "D"); // D depends on C

    auto [valid, order] = g.valid_build();

    EXPECT_TRUE(valid);
    EXPECT_EQ(order.size(), 4);
    EXPECT_TRUE(appears_before(order, "A", "B"));
    EXPECT_TRUE(appears_before(order, "A", "C"));
    EXPECT_TRUE(appears_before(order, "B", "D"));
    EXPECT_TRUE(appears_before(order, "C", "D"));
}

TEST(GraphTest, CycleIsInvalid) {
    Graph g;
    g.add_target({"A", {}, "cmd_a"});
    g.add_target({"B", {}, "cmd_b"});

    g.add_edge("A", "B");
    g.add_edge("B", "A");

    auto [valid, order] = g.valid_build();

    EXPECT_FALSE(valid);
}

TEST(GraphTest, IsolatedTargetIsIncluded) {
    Graph g;
    g.add_target({"A", {}, "cmd_a"});
    g.add_target({"B", {}, "cmd_b"});

    auto [valid, order] = g.valid_build();

    EXPECT_TRUE(valid);
    EXPECT_EQ(order.size(), 2);
}

TEST(GraphTest, AddEdgeThrowsOnUnknownTarget) {
    Graph g;
    g.add_target({"A", {}, "cmd_a"});

    EXPECT_THROW(g.add_edge("A", "B"), std::runtime_error);
    EXPECT_THROW(g.add_edge("Z", "A"), std::runtime_error);
}