#include <gtest/gtest.h>
#include "test_types.hpp"

// 3. Enums
TEST(DebugDeriveTest, Enums) {
    // Registered enum with DEBUG_ENUM
    EXPECT_EQ(to_debug_string(TestColor::Green), "TestColor::Green");

    // Unregistered enum fallback to Name(underlying_value)
    EXPECT_EQ(to_debug_string(North), "TestDirection(0)");
    EXPECT_EQ(to_debug_string(South), "TestDirection(1)");
}
