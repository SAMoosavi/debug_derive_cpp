#include <gtest/gtest.h>
#include "test_types.hpp"

// 19. Template class primary (intrusive). Key matches neither specialization,
// so this exercises the primary template's own reflector.
TEST(DebugDeriveTest, TemplateContainerPrimary) {
    Container<double, int> c{3.5, 42};
    EXPECT_EQ(to_debug_string(c),
        "Container<double, int> {\n  key: 3.5,\n  value: 42\n}");
}

// 20. Partial specialization on std::string (intrusive). The distinct member
// set (name/payload/tag, not key/value) proves the specialization runs.
TEST(DebugDeriveTest, TemplateContainerStringSpec) {
    Container<std::string, int> c{"cache", 7, 1};
    std::string expected =
        "Container<std::string, int> {\n"
        "  name: \"cache\",\n"
        "  payload: 7,\n"
        "  tag: 1\n"
        "}";
    EXPECT_EQ(to_debug_string(c), expected);
}

// 21. Partial specialization on int (non-intrusive hand-written reflector).
TEST(DebugDeriveTest, TemplateContainerIntSpec) {
    Container<int, std::string> c{9, "nine"};
    std::string expected =
        "Container<int, std::string> {\n"
        "  id: 9,\n"
        "  data: \"nine\"\n"
        "}";
    EXPECT_EQ(to_debug_string(c), expected);
}

// 22. Nested templates recurse; inner value uses the int specialization.
TEST(DebugDeriveTest, TemplateContainerNested) {
    Container<double, Container<int, int>> c{1.5, {3, 4}};
    std::string expected =
        "Container<double, Container<int, int>> {\n"
        "  key: 1.5,\n"
        "  value: Container<int, int> {\n"
        "    id: 3,\n"
        "    data: 4\n"
        "  }\n"
        "}";
    EXPECT_EQ(to_debug_string(c), expected);
}

TEST(DebugDeriveTest, TemplateContainerSimplePoint) {
    // AAAAA matches no specialization: primary template, key auto-prints
    // (including its raw pointer member).
    Container<AAAAA, int> c{{nullptr}, 3};
    EXPECT_EQ(to_debug_string(c),
        "Container<AAAAA, int> {\n  key: AAAAA {\n    tt: nullptr\n  },\n  value: 3\n}");

    // SimplePoint key selects the SimplePoint partial specialization.
    Container<SimplePoint, int> s{{10, 20}, 5};
    EXPECT_EQ(to_debug_string(s),
        "Container<SimplePoint, int> {\n"
        "  id: SimplePoint {\n"
        "    x: 10,\n"
        "    y: 20\n"
        "  },\n"
        "  data: 5\n"
        "}");
}
