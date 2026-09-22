#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <optional>
#include <tuple>
#include <memory>
#include <map>
#include "debug_derive.hpp"

// ============================================================================
// Test Types
// ============================================================================

struct EmptyStruct {
    DEBUG_FIELDS()
};

struct ExternalEmpty {};
DEBUG_STRUCT(ExternalEmpty)

struct SimplePoint {
    int x;
    int y;
    DEBUG_FIELDS(x, y)
};

struct AddressTest {
    std::string city;
    int zip;
    DEBUG_FIELDS(city, zip)
};

template<typename A>
struct UserTest {
    std::string name;
    A age;
    AddressTest address;
    DEBUG_FIELDS(name, age, address)
};

struct AAAAA {
    int* tt;
};

// Template class with partial specializations, one per registration style.
template <typename Key, typename Value>
class Container {
public:
    Key key;
    Value value;
    DEBUG_FIELDS(key, value)
};

template <typename Value>
class Container<std::string, Value> {
public:
    std::string name;
    Value payload;
    int tag;
    DEBUG_FIELDS(name, payload, tag)
};

template <typename Value>
class Container<int, Value> {
public:
    int id;
    Value data;
    DEBUG_FIELDS()

};

template <typename Value>
class Container<SimplePoint, Value> {
public:
    SimplePoint id;
    Value data;
    DEBUG_FIELDS()

};

struct Level4 {
    int val4;
    DEBUG_FIELDS(val4)
};

struct Level3 {
    Level4 l4;
    DEBUG_FIELDS(l4)
};

struct Level2 {
    Level3 l3;
    DEBUG_FIELDS(l3)
};

struct Level1 {
    Level2 l2;
    DEBUG_FIELDS(l2)
};

class EncapsulatedData {
private:
    std::string secret;
    int pin;

public:
    EncapsulatedData(std::string s, int p) : secret(std::move(s)), pin(p) {}
    DEBUG_FIELDS(secret, pin)
};

enum class TestColor { Red, Green, Blue };
DEBUG_ENUM(TestColor, Red, Green, Blue)

enum TestDirection { North, South };

struct CyclicNode {
    int id;
    std::shared_ptr<CyclicNode> next;
    DEBUG_FIELDS(id, next)
};

// Zero-argument auto reflection (Section 4b of debug_derive.hpp)
struct AutoAddress {
    std::string city;
    int zip;
    DEBUG_FIELDS()
};

struct AutoUser {
    std::string name;
    int age;
    AutoAddress address;
    DEBUG_FIELDS()
};

struct ThirdPartyAuto {
    int x;
    int y;
};
DEBUG_AUTO(ThirdPartyAuto)

// No macro at all: engine auto-discovers public aggregates.
struct BareAuto {
    int x;
    int y;
};

struct AutoEmpty {
    DEBUG_FIELDS()
};

// Explicit listing wins over auto-discovery (note reversed order).
struct ExplicitOrder {
    int b;
    int a;
    DEBUG_FIELDS(a, b)
};

// Feature 2: automatic recursive fallback — bare aggregates nested in
// explicit parents (no macro on the nested type, no per-type duplication).
struct BareInner {
    int v;
};

struct BareHolder {
    AddressTest addr;
    int tag;
};

struct ExplicitOuter {
    BareInner inner;
    int n;
    DEBUG_FIELDS(inner, n)
};

struct MixedNested {
    int id;
    std::string name;
    BareInner inner;
    AddressTest addr;
    DEBUG_FIELDS(id, name, inner, addr)
};

struct BareLevel3 {
    int z;
};

struct BareLevel2 {
    BareLevel3 c;
    int y;
};

struct ExplicitLevel1 {
    BareLevel2 b;
    int x;
    DEBUG_FIELDS(b, x)
};

static_assert(debug_derive::IsAutoReflectable<BareInner>);
static_assert(!debug_derive::IsDebugReflectable<BareInner>);
static_assert(debug_derive::uses_generic_auto_v<BareInner>);
static_assert(debug_derive::uses_generic_auto_v<const BareInner&>);
static_assert(!debug_derive::uses_generic_auto_v<SimplePoint>);
static_assert(!debug_derive::uses_generic_auto_v<ExplicitOuter>);
static_assert(!debug_derive::uses_generic_auto_v<int>);
static_assert(!debug_derive::IsAutoReflectable<EncapsulatedData>);
static_assert(!debug_derive::IsAutoReflectable<std::vector<int>>);
// Empty DEBUG_FIELDS() registers an explicit reflector that forwards to the
// auto engine, so explicit registration (and precedence) still holds.
static_assert(debug_derive::IsDebugReflectable<AutoUser>);
static_assert(!debug_derive::IsAutoReflectable<AutoUser>);
static_assert(!debug_derive::IsAutoReflectable<SimplePoint>);

// Canonical private/protected fixture: int a is private-implicit (class
// default access), b is protected, q is private, c is public.
class A {
    int a;
protected:
    int b;
private:
    int q;
public:
    int c;
    A(int a_, int b_, int q_, int c_) : a(a_), b(b_), q(q_), c(c_) {}
    DEBUG_FIELDS(a, b, q, c)
};

static_assert(debug_derive::IsDebugReflectable<A>);
static_assert(!debug_derive::IsAutoReflectable<A>);

class NestedPrivateHolder {
    A inner;
    int tag;
public:
    NestedPrivateHolder(A in, int t) : inner(in), tag(t) {}
    DEBUG_FIELDS(inner, tag)
};

class MultiPrimitivePrivate {
    int i;
    double d;
    bool flag;
    std::string s;
public:
    MultiPrimitivePrivate(int i_, double d_, bool f_, std::string s_)
        : i(i_), d(d_), flag(f_), s(std::move(s_)) {}
    DEBUG_FIELDS(i, d, flag, s)
};

class ZeroArgNonAggregate {
    // ponytail: intentionally unlisted (fallback test) → maybe_unused keeps
    // -Wunused-private-field clean on Clang.
    [[maybe_unused]] int x;
public:
    explicit ZeroArgNonAggregate(int v) : x(v) {}
    DEBUG_FIELDS()
};

// Private holder over a shared subtree: exercises private dispatch through
// the cycle-128 / depth-32 guards with zero new engine code.
class PrivateCycleHolder {
    std::shared_ptr<CyclicNode> node;
public:
    explicit PrivateCycleHolder(std::shared_ptr<CyclicNode> n) : node(std::move(n)) {}
    DEBUG_FIELDS(node)
};

// Non-aggregate, unregistered, unstreamable → runtime `<unformattable T>`.
struct Unprintable {
    Unprintable(int v_) : value(v_) {}
private:
    // ponytail: never read (fallback prints the marker) → maybe_unused keeps
    // -Wunused-private-field clean on Clang.
    [[maybe_unused]] int value;
};

class PrivateUnformattableHolder {
    Unprintable inner;
    int tag;
public:
    PrivateUnformattableHolder(Unprintable in, int t) : inner(in), tag(t) {}
    DEBUG_FIELDS(inner, tag)
};
