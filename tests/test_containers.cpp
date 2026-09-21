#include <gtest/gtest.h>
#include "test_types.hpp"

// 6. Containers and Vectors
TEST(DebugDeriveTest, ContainersAndVectors) {
    std::vector<int> empty_vec;
    EXPECT_EQ(to_debug_string(empty_vec), "[]");

    std::vector<int> vec{1, 2, 3};
    std::string expected_vec = 
        "[\n"
        "  1,\n"
        "  2,\n"
        "  3\n"
        "]";
    EXPECT_EQ(to_debug_string(vec), expected_vec);

    std::array<std::string, 2> arr{"apple", "banana"};
    std::string expected_arr = 
        "[\n"
        "  \"apple\",\n"
        "  \"banana\"\n"
        "]";
    EXPECT_EQ(to_debug_string(arr), expected_arr);
}

TEST(DebugDeriveTest, MAP) {
    std::map<int,int> empty_vec;
    EXPECT_EQ(to_debug_string(empty_vec), "[]");

    std::map<int,int> vec{{1, 2}, {3, 4}};
    std::string expected_vec =
        "[\n"
        "  (1, 2),\n"
        "  (3, 4)\n"
        "]";
    EXPECT_EQ(to_debug_string(vec), expected_vec);
}


// 7. Optionals
TEST(DebugDeriveTest, Optionals) {
    std::optional<int> none_val = std::nullopt;
    EXPECT_EQ(to_debug_string(none_val), "None");

    std::optional<int> some_val = 42;
    EXPECT_EQ(to_debug_string(some_val), "Some(42)");

    std::optional<SimplePoint> some_struct = SimplePoint{10, 20};
    std::string expected_some_struct = 
        "Some(SimplePoint {\n"
        "  x: 10,\n"
        "  y: 20\n"
        "})";
    EXPECT_EQ(to_debug_string(some_struct), expected_some_struct);
}

// 8. Pairs and Tuples
TEST(DebugDeriveTest, PairsAndTuples) {
    std::pair<std::string, int> p{"counter", 100};
    EXPECT_EQ(to_debug_string(p), "(\"counter\", 100)");

    std::tuple<> empty_tup;
    EXPECT_EQ(to_debug_string(empty_tup), "()");

    auto tup = std::make_tuple(1, std::string("test"), true);
    EXPECT_EQ(to_debug_string(tup), "(1, \"test\", true)");
}
