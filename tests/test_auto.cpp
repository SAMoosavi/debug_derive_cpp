#include <gtest/gtest.h>
#include "test_types.hpp"

// 13. Automatic reflection: intrusive zero-arg macro, real member names
TEST(DebugDeriveTest, AutoIntrusive) {
    AutoUser user{"Alice", 30, {"Amsterdam", 1011}};
    std::string expected =
        "AutoUser {\n"
        "  name: \"Alice\",\n"
        "  age: 30,\n"
        "  address: AutoAddress {\n"
        "    city: \"Amsterdam\",\n"
        "    zip: 1011\n"
        "  }\n"
        "}";
    EXPECT_EQ(to_debug_string(user), expected);
}

// 14. Automatic reflection: non-intrusive DEBUG_AUTO on a third-party type
TEST(DebugDeriveTest, AutoNonIntrusive) {
    ThirdPartyAuto p{10, 20};
    EXPECT_EQ(to_debug_string(p), "ThirdPartyAuto {\n  x: 10,\n  y: 20\n}");
}

// 15. Automatic reflection: no macro at all on a public aggregate
TEST(DebugDeriveTest, AutoWithoutAnyMacro) {
    BareAuto b{1, 2};
    EXPECT_EQ(to_debug_string(b), "BareAuto {\n  x: 1,\n  y: 2\n}");
}

// 16. Automatic reflection: empty aggregate prints as empty
TEST(DebugDeriveTest, AutoEmpty) {
    AutoEmpty e;
    EXPECT_EQ(to_debug_string(e), "AutoEmpty {}");
}

// 17. Automatic reflection: nesting inside containers and optionals
TEST(DebugDeriveTest, AutoNestedContainers) {
    std::vector<AutoAddress> v{{"Amsterdam", 1011}, {"Berlin", 10115}};
    std::string expected =
        "[\n"
        "  AutoAddress {\n"
        "    city: \"Amsterdam\",\n"
        "    zip: 1011\n"
        "  },\n"
        "  AutoAddress {\n"
        "    city: \"Berlin\",\n"
        "    zip: 10115\n"
        "  }\n"
        "]";
    EXPECT_EQ(to_debug_string(v), expected);

    std::optional<AutoAddress> some = AutoAddress{"Rome", 100};
    EXPECT_EQ(to_debug_string(some),
        "Some(AutoAddress {\n  city: \"Rome\",\n  zip: 100\n})");
}

// 18. Explicit field listing takes precedence over auto-discovery
TEST(DebugDeriveTest, ExplicitRegistrationTakesPrecedence) {
    ExplicitOrder e{1, 2}; // declaration order: b = 1, a = 2
    EXPECT_EQ(to_debug_string(e), "ExplicitOrder {\n  a: 2,\n  b: 1\n}");
}
