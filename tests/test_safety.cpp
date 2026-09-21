#include <gtest/gtest.h>
#include "test_types.hpp"

// 11. Cycle Detection in Recursive Structures
TEST(DebugDeriveTest, CycleDetection) {
    auto n1 = std::make_shared<CyclicNode>(CyclicNode{1, nullptr});
    auto n2 = std::make_shared<CyclicNode>(CyclicNode{2, nullptr});
    n1->next = n2;
    n2->next = n1; // Cycle!

    std::string output = to_debug_string(*n1);
    
    // Must contain "<cycle: CyclicNode@"
    EXPECT_NE(output.find("<cycle: CyclicNode@"), std::string::npos);
    EXPECT_NE(output.find("id: 1"), std::string::npos);
    EXPECT_NE(output.find("id: 2"), std::string::npos);

    
}

// 12. Depth Limiting
TEST(DebugDeriveTest, DepthLimiting) {
    // Build a deep non-cyclic chain
    std::shared_ptr<CyclicNode> head = std::make_shared<CyclicNode>(CyclicNode{0, nullptr});
    auto curr = head;
    for (int i = 1; i < 40; ++i) {
        curr->next = std::make_shared<CyclicNode>(CyclicNode{i, nullptr});
        curr = curr->next;
    }

    std::string output = to_debug_string(*head);
    EXPECT_NE(output.find("<recursion limit reached>"), std::string::npos);
}
