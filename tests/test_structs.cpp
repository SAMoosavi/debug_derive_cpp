#include <gtest/gtest.h>
#include "test_types.hpp"

// 4. Empty Structs
TEST(DebugDeriveTest, EmptyStructs) {
    EmptyStruct e1;
    EXPECT_EQ(to_debug_string(e1), "EmptyStruct {}");

    ExternalEmpty e2;
    EXPECT_EQ(to_debug_string(e2), "ExternalEmpty {}");
}

// 5. Nested Structs
TEST(DebugDeriveTest, NestedStructs) {
    UserTest<int> user{"Alice", 30, {"Amsterdam", 1011}};
    std::string expected = 
        "UserTest<int> {\n"
        "  name: \"Alice\",\n"
        "  age: 30,\n"
        "  address: AddressTest {\n"
        "    city: \"Amsterdam\",\n"
        "    zip: 1011\n"
        "  }\n"
        "}";
    EXPECT_EQ(to_debug_string(user), expected);
}

// 9. Deeply Nested Structures
TEST(DebugDeriveTest, DeeplyNestedStructures) {
    Level1 root{{{ {42} }}};
    std::string expected = 
        "Level1 {\n"
        "  l2: Level2 {\n"
        "    l3: Level3 {\n"
        "      l4: Level4 {\n"
        "        val4: 42\n"
        "      }\n"
        "    }\n"
        "  }\n"
        "}";
    EXPECT_EQ(to_debug_string(root), expected);
}

// 10. Private Members Encapsulation
TEST(DebugDeriveTest, PrivateMembers) {
    EncapsulatedData enc{"super-secret-token", 1234};
    std::string expected = 
        "EncapsulatedData {\n"
        "  secret: \"super-secret-token\",\n"
        "  pin: 1234\n"
        "}";
    EXPECT_EQ(to_debug_string(enc), expected);
}
