#include <gtest/gtest.h>
#include "test_types.hpp"

// 1. Primitive Values
TEST(DebugDeriveTest, PrimitiveValues) {
    EXPECT_EQ(to_debug_string(true), "true");
    EXPECT_EQ(to_debug_string(false), "false");
    EXPECT_EQ(to_debug_string(42), "42");
    EXPECT_EQ(to_debug_string(-999), "-999");
    EXPECT_EQ(to_debug_string(123456789012345LL), "123456789012345");
    EXPECT_EQ(to_debug_string(3.14), "3.14");
    EXPECT_EQ(to_debug_string('Z'), "'Z'");
    EXPECT_EQ(to_debug_string('\n'), "'\\n'");
    EXPECT_EQ(to_debug_string('\t'), "'\\t'");
    EXPECT_EQ(to_debug_string('\''), "'\\''");
    EXPECT_EQ(to_debug_string(static_cast<uint8_t>(65)), "65");


}
