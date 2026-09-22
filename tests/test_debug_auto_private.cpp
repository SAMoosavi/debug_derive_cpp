// Quarantined TU: zero-arg DEBUG_FIELDS() auto-printing private/protected.
//
// The `#define private public` hack below is UNDEFINED BEHAVIOR by the
// standard and is confined to this file only: every include (stdlib, GTest,
// the library header) comes first, the hack is active solely around the demo
// type definitions, and it is undone immediately after. Active in Debug only
// (#ifndef NDEBUG); Release compiles exactly like today (plain classes, the
// zero-arg macro falls back to `Type {}`). Never copy this pattern into
// headers, other TUs, or global build flags.

#include <gtest/gtest.h>

#include <string>

#include "debug_derive.hpp"

#ifndef NDEBUG
#pragma push_macro("private")
#pragma push_macro("protected")
#undef private
#undef protected
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wkeyword-macro"
#endif
#define private public
#define protected public
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif

// Pure data: no user ctor, so with the hack (struct => first section already
// public, remaining labels macro-replaced) these become aggregates in Debug
// and the zero-arg auto engine discovers every member with its real name.
struct DebugAutoA {
private:
    // ponytail: maybe_unused keeps -Wunused-private-field clean on Clang in
    // Release, where the hack is off and the fallback test never reads these.
    [[maybe_unused]] int a;
protected:
    int b;
private:
    [[maybe_unused]] int q;
public:
    int c;
    DEBUG_FIELDS()
};

struct DebugAutoMulti {
private:
    [[maybe_unused]] int i;
    [[maybe_unused]] double d;
protected:
    bool flag;
public:
    std::string s;
    DEBUG_FIELDS()
};

#ifndef NDEBUG
#pragma pop_macro("protected")
#pragma pop_macro("private")
#endif

#ifndef NDEBUG
static_assert(std::is_aggregate_v<DebugAutoA>);
static_assert(std::is_aggregate_v<DebugAutoMulti>);
#else
static_assert(!std::is_aggregate_v<DebugAutoA>);
static_assert(!std::is_aggregate_v<DebugAutoMulti>);
#endif

#ifndef NDEBUG
TEST(DebugDeriveAutoPrivate, ZeroArgPrintsAllSections) {
    DebugAutoA v{1, 2, 3, 4};
    EXPECT_EQ(to_debug_string(v), "DebugAutoA {\n  a: 1,\n  b: 2,\n  q: 3,\n  c: 4\n}");
}

TEST(DebugDeriveAutoPrivate, ZeroArgMultiPrimitive) {
    DebugAutoMulti m{7, 2.5, true, "hi"};
    const std::string out = to_debug_string(m);
    EXPECT_NE(out.find("i: 7"), std::string::npos);
    EXPECT_NE(out.find("d: 2.5"), std::string::npos);
    EXPECT_NE(out.find("flag: true"), std::string::npos);
    EXPECT_NE(out.find("s: \"hi\""), std::string::npos);
    EXPECT_EQ(out, "DebugAutoMulti {\n  i: 7,\n  d: 2.5,\n  flag: true,\n  s: \"hi\"\n}");
}
#else
// Release: hack off, classes keep their private sections, so they are not
// aggregates and zero-arg stays the documented no-op fallback (`Type {}`).
TEST(DebugDeriveAutoPrivate, ReleaseFallbackDocumentsNoOp) {
    DebugAutoA v{};
    EXPECT_EQ(to_debug_string(v), "DebugAutoA {}");
    DebugAutoMulti m{};
    EXPECT_EQ(to_debug_string(m), "DebugAutoMulti {}");
}
#endif
