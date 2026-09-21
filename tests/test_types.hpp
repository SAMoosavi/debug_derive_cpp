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

static_assert(debug_derive::IsAutoReflectable<BareAuto>);
static_assert(!debug_derive::IsAutoReflectable<EncapsulatedData>);
static_assert(!debug_derive::IsAutoReflectable<std::vector<int>>);
// Empty DEBUG_FIELDS() registers an explicit reflector that forwards to the
// auto engine, so explicit registration (and precedence) still holds.
static_assert(debug_derive::IsDebugReflectable<AutoUser>);
static_assert(!debug_derive::IsAutoReflectable<AutoUser>);
static_assert(!debug_derive::IsAutoReflectable<SimplePoint>);
