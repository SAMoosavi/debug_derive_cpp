#include <gtest/gtest.h>
#include "test_types.hpp"

// 2. Strings and Escaping
TEST(DebugDeriveTest, StringsAndEscaping) {
    EXPECT_EQ(to_debug_string(std::string("Hello World")), "\"Hello World\"");
    EXPECT_EQ(to_debug_string("C-String"), "\"C-String\"");
    EXPECT_EQ(to_debug_string(std::string_view("StringView")), "\"StringView\"");
    
    const char* null_str = nullptr;
    EXPECT_EQ(to_debug_string(null_str), "nullptr");

    std::string with_escapes = "Line1\nLine2\t\"Quotes\"\\Backslash";
    EXPECT_EQ(to_debug_string(with_escapes), "\"Line1\\nLine2\\t\\\"Quotes\\\"\\\\Backslash\"");
}
