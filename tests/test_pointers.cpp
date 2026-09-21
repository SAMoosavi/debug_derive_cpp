#include <gtest/gtest.h>
#include "test_types.hpp"

// 23. Raw pointers: null prints nullptr, values dereference to scalars.
TEST(DebugDeriveTest, RawPointerNullAndScalars) {
    int* null_p = nullptr;
    EXPECT_EQ(to_debug_string(null_p), "nullptr");

    int x = 42;
    int* p = &x;
    EXPECT_EQ(to_debug_string(p), "42");

    const int cx = -7;
    const int* cp = &cx;
    EXPECT_EQ(to_debug_string(cp), "-7");

    double d = 3.5;
    double* dp = &d;
    EXPECT_EQ(to_debug_string(dp), "3.5");
}

// 24. Raw pointers dereference into strings, containers, and structs
// (explicit and auto-registered alike).
TEST(DebugDeriveTest, RawPointerDerefValues) {
    std::string s = "hi";
    std::string* sp = &s;
    EXPECT_EQ(to_debug_string(sp), "\"hi\"");

    std::vector<int> v{1, 2};
    std::vector<int>* vp = &v;
    EXPECT_EQ(to_debug_string(vp), "[\n  1,\n  2\n]");

    SimplePoint pt{3, 4};
    SimplePoint* pp = &pt;
    EXPECT_EQ(to_debug_string(pp), "SimplePoint {\n  x: 3,\n  y: 4\n}");

    BareAuto b{5, 6};
    BareAuto* bp = &b;
    EXPECT_EQ(to_debug_string(bp), "BareAuto {\n  x: 5,\n  y: 6\n}");
}

// 25. Raw void* and function pointers cannot dereference: keep the address.
TEST(DebugDeriveTest, RawPointerUndereferenceable) {
    int x = 42;
    void* vp = &x;
    std::ostringstream voss;
    voss << reinterpret_cast<const void*>(&x);
    EXPECT_EQ(to_debug_string(vp), voss.str());

    auto* fn = +[](int v) { return v + 1; };
    std::ostringstream foss;
    foss << reinterpret_cast<const void*>(fn);
    EXPECT_EQ(to_debug_string(fn), foss.str());
}

// 26. Smart pointers: null prints nullptr, values dereference (pinned).
TEST(DebugDeriveTest, SmartPointerNullAndValue) {
    std::shared_ptr<int> null_sp;
    EXPECT_EQ(to_debug_string(null_sp), "nullptr");

    auto sp = std::make_shared<int>(7);
    EXPECT_EQ(to_debug_string(sp), "7");

    auto up = std::make_unique<std::string>("up");
    EXPECT_EQ(to_debug_string(up), "\"up\"");

    std::unique_ptr<int> null_up;
    EXPECT_EQ(to_debug_string(null_up), "nullptr");
}
