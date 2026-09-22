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

// Feature 2: automatic recursive fallback — explicit parent, auto child.
// 19. Primitive member through the generic auto path (no macro on BareInner)
TEST(DebugDeriveTest, NestedPrimitiveMember) {
    EXPECT_EQ(to_debug_string(BareInner{42}), "BareInner {\n  v: 42\n}");
}

// 20. Nested type WITH explicit DEBUG inside an auto parent: byte-identical
// to its standalone explicit format.
TEST(DebugDeriveTest, NestedWithExplicitDebug) {
    BareHolder h{{"Oslo", 500}, 7};
    std::string expected =
        "BareHolder {\n"
        "  addr: AddressTest {\n"
        "    city: \"Oslo\",\n"
        "    zip: 500\n"
        "  },\n"
        "  tag: 7\n"
        "}";
    EXPECT_EQ(to_debug_string(h), expected);
}

// 21. Nested type WITHOUT any DEBUG (public aggregate) inside an explicit
// parent: generic auto recursion, same format.
TEST(DebugDeriveTest, NestedWithoutDebug) {
    ExplicitOuter o{{7}, 9};
    std::string expected =
        "ExplicitOuter {\n"
        "  inner: BareInner {\n"
        "    v: 7\n"
        "  },\n"
        "  n: 9\n"
        "}";
    EXPECT_EQ(to_debug_string(o), expected);
}

// 22. Three-level nesting: explicit -> bare -> bare.
TEST(DebugDeriveTest, ThreeLevelNesting) {
    ExplicitLevel1 root{{{{5}}, 6}, 7};
    std::string expected =
        "ExplicitLevel1 {\n"
        "  b: BareLevel2 {\n"
        "    c: BareLevel3 {\n"
        "      z: 5\n"
        "    },\n"
        "    y: 6\n"
        "  },\n"
        "  x: 7\n"
        "}";
    EXPECT_EQ(to_debug_string(root), expected);
}

// 23. Mixed primitive + bare + explicit members; const/ref/value identical.
TEST(DebugDeriveTest, MixedPrimitiveAndUserDefined) {
    MixedNested m{1, "Li", {2}, {"Oslo", 500}};
    std::string expected =
        "MixedNested {\n"
        "  id: 1,\n"
        "  name: \"Li\",\n"
        "  inner: BareInner {\n"
        "    v: 2\n"
        "  },\n"
        "  addr: AddressTest {\n"
        "    city: \"Oslo\",\n"
        "    zip: 500\n"
        "  }\n"
        "}";
    const MixedNested& cref = m;
    EXPECT_EQ(to_debug_string(m), expected);
    EXPECT_EQ(to_debug_string(cref), expected);
    MixedNested copy = m;
    EXPECT_EQ(to_debug_string(copy), expected);
}
