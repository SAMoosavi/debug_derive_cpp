/**
 * @file debug_derive.hpp
 * @brief Header-only C++20 `#[derive(Debug)]` clone: pretty debug-printing
 *        for structs, containers, optionals, pointers, tuples, and enums.
 *
 * Usage example:
 *   #include "debug_derive.hpp"
 *   struct Point { int x; int y; DEBUG_FIELDS(x, y) };
 *   Point p{1, 2};
 *   std::string s = to_debug_string(p);
 *   debug_print(p);
 *
 * Requirements: C++20, GCC or Clang, header-only, no dependencies.
 */
#pragma once

#define DEBUG_DERIVE_VERSION_MAJOR 0
#define DEBUG_DERIVE_VERSION_MINOR 1
#define DEBUG_DERIVE_VERSION_PATCH 0
#define DEBUG_DERIVE_VERSION_STRING "0.1.0"

#if __cplusplus < 202002L
#error "debug_derive.hpp requires C++20 (__cplusplus >= 202002L)"
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <optional>
#include <tuple>
#include <memory>
#include <utility>
#include <type_traits>
#include <concepts>
#include <cstdint>
#include <cstdio>

// ============================================================================
// Section 1: Preprocessor Variadic Metaprogramming (Supports 0 to 64 fields)
// ============================================================================

#define CPPDERIVE_PP_EXPAND(x) x

#define CPPDERIVE_PP_ARG_N( \
    _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9,  _10, \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
    _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
    _61, _62, _63, _64,  N, ...) N

#define CPPDERIVE_PP_RSEQ_N \
    64, 63, 62, 61, 60, 59, 58, 57, 56, 55, \
    54, 53, 52, 51, 50, 49, 48, 47, 46, 45, \
    44, 43, 42, 41, 40, 39, 38, 37, 36, 35, \
    34, 33, 32, 31, 30, 29, 28, 27, 26, 25, \
    24, 23, 22, 21, 20, 19, 18, 17, 16, 15, \
    14, 13, 12, 11, 10,  9,  8,  7,  6,  5, \
     4,  3,  2,  1,  0

#define CPPDERIVE_PP_NARG_IMPL(...) CPPDERIVE_PP_ARG_N(__VA_ARGS__)
#define CPPDERIVE_PP_NARG(...) CPPDERIVE_PP_NARG_IMPL(__VA_ARGS__, CPPDERIVE_PP_RSEQ_N)

#define CPPDERIVE_PP_CONCAT_2(a, b) a ## b
#define CPPDERIVE_PP_CONCAT(a, b) CPPDERIVE_PP_CONCAT_2(a, b)

#define CPPDERIVE_PP_MAP_0(m, d)
#define CPPDERIVE_PP_MAP_1(m, d, x) m(d, x)
#define CPPDERIVE_PP_MAP_2(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_1(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_3(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_2(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_4(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_3(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_5(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_4(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_6(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_5(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_7(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_6(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_8(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_7(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_9(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_8(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_10(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_9(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_11(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_10(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_12(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_11(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_13(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_12(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_14(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_13(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_15(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_14(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_16(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_15(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_17(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_16(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_18(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_17(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_19(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_18(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_20(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_19(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_21(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_20(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_22(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_21(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_23(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_22(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_24(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_23(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_25(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_24(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_26(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_25(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_27(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_26(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_28(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_27(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_29(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_28(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_30(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_29(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_31(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_30(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_32(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_31(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_33(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_32(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_34(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_33(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_35(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_34(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_36(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_35(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_37(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_36(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_38(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_37(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_39(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_38(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_40(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_39(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_41(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_40(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_42(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_41(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_43(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_42(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_44(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_43(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_45(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_44(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_46(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_45(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_47(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_46(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_48(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_47(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_49(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_48(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_50(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_49(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_51(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_50(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_52(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_51(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_53(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_52(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_54(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_53(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_55(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_54(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_56(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_55(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_57(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_56(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_58(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_57(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_59(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_58(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_60(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_59(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_61(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_60(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_62(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_61(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_63(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_62(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_MAP_64(m, d, x, ...) m(d, x) CPPDERIVE_PP_MAP_63(m, d, __VA_ARGS__)

#define CPPDERIVE_PP_FOR_EACH_IMPL(count, m, d, ...) CPPDERIVE_PP_CONCAT(CPPDERIVE_PP_MAP_, count)(m, d, __VA_ARGS__)
#define CPPDERIVE_PP_FOR_EACH(m, d, ...) CPPDERIVE_PP_FOR_EACH_IMPL(CPPDERIVE_PP_NARG(__VA_ARGS__), m, d, __VA_ARGS__)

// Member visitor macros
#define CPPDERIVE_VISIT_THIS(d, f) visitor(#f, this->f);
#define CPPDERIVE_VISIT_OBJ(obj, f) visitor(#f, obj.f);
#define CPPDERIVE_ENUM_CASE(Type, val) case Type::val: return #Type "::" #val;

namespace debug_derive {

// Forward declare friend access struct for private members
struct DebugAccess;

} // namespace debug_derive

// ============================================================================
// Section 2: Registration Macros (Intrusive and Non-Intrusive)
// ============================================================================

/**
 * @brief Intrusive reflection macro. Place inside the struct or class body.
 *
 * With explicit field names, prints exactly those fields (works with private
 * members, preserves aggregate initialization for structs).
 * With zero arguments, auto-discovers all fields of public aggregates
 * (real member names, no dependency). Non-aggregates with zero args print
 * as `Type {}` — list their fields explicitly instead.
 *
 * Private/protected members print via the canonical explicit list: the
 * macro befriends `debug_derive::DebugAccess`, so `DEBUG_FIELDS(a, b, q, c)`
 * reaches every access section with no engine change. True zero-arg
 * auto-naming of privates is impossible in standard C++20/C++23 (see
 * Section 9). Never `#define private public` in the default path (UB);
 * that variant is opt-in only, quarantined to its own TU (see docs).
 *
 * Example:
 *   struct Address {
 *       std::string city;
 *       int zip;
 *       DEBUG_FIELDS()          // auto: city, zip
 *   };
 */
#define DEBUG_FIELDS(...) \
    friend struct ::debug_derive::DebugAccess; \
    template <typename Visitor_> \
    void __debug_reflect([[maybe_unused]] Visitor_&& visitor) const { \
        __VA_OPT__(CPPDERIVE_PP_FOR_EACH(CPPDERIVE_VISIT_THIS, _, __VA_ARGS__) return;) \
        ::debug_derive::detail::auto_impl::visit_auto(*this, std::forward<Visitor_>(visitor)); \
    }

/**
 * @brief Non-intrusive reflection macro. Place outside the struct or class definition.
 *
 * Useful for 3rd-party types, standard structures, or code where you cannot
 * edit the class definition directly. With zero field arguments, auto-discovers
 * all fields of public aggregates.
 *
 * Example:
 *   struct Point { int x; int y; };
 *   DEBUG_STRUCT(Point)           // auto: x, y
 */
#define DEBUG_STRUCT(Type, ...) \
    template <typename Visitor_> \
    void __debug_reflect([[maybe_unused]] const Type& obj, [[maybe_unused]] Visitor_&& visitor) { \
        __VA_OPT__(CPPDERIVE_PP_FOR_EACH(CPPDERIVE_VISIT_OBJ, obj, __VA_ARGS__) return;) \
        ::debug_derive::detail::auto_impl::visit_auto(obj, std::forward<Visitor_>(visitor)); \
    } \
    constexpr std::string_view __debug_type_name_tag(const Type*) { \
        return #Type; \
    }

/**
 * @brief Non-intrusive auto reflection. Equivalent to `DEBUG_STRUCT(Type)`
 * with no field list: auto-discovers all fields of a public aggregate.
 *
 * Example:
 *   struct Point { int x; int y; };
 *   DEBUG_AUTO(Point)
 */
#define DEBUG_AUTO(Type) DEBUG_STRUCT(Type)

/**
 * @brief Enum symbol registration macro.
 *
 * Optional: enables printing symbolic names (e.g., `Status::Active`) instead
 * of `Status(0)`.
 *
 * Example:
 *   enum class Status { Active, Inactive };
 *   DEBUG_ENUM(Status, Active, Inactive)
 */
#define DEBUG_ENUM(Type, ...) \
    constexpr std::string_view __debug_enum_to_string(Type val) { \
        switch (val) { \
            __VA_OPT__(CPPDERIVE_PP_FOR_EACH(CPPDERIVE_ENUM_CASE, Type, __VA_ARGS__)) \
            default: return {}; \
        } \
    }

/**
 * @brief Declare friendship to allow external DEBUG_STRUCT to access private members.
 */
#define DEBUG_ALLOW_EXTERNAL_FRIEND(Type) \
    template <typename Visitor_> \
    friend void __debug_reflect(const Type& obj, Visitor_&& visitor);

namespace debug_derive {

// ============================================================================
// Section 3: Zero-RTTI Compile-Time Type Name Resolution
// ============================================================================

template <typename T>
constexpr std::string_view get_raw_type_name() {
#if defined(__clang__)
    std::string_view p = __PRETTY_FUNCTION__;
    auto start = p.find("[T = ");
    if (start == std::string_view::npos) return "UnknownType";
    start += 5;
    auto end = p.find(']', start);
    return p.substr(start, end - start);
#elif defined(__GNUC__)
    std::string_view p = __PRETTY_FUNCTION__;
    auto start = p.find("T = ");
    if (start == std::string_view::npos) return "UnknownType";
    start += 4;
    auto end = p.find_first_of(";]", start);
    return p.substr(start, end - start);
#elif defined(_MSC_VER)
    std::string_view p = __FUNCSIG__;
    auto start = p.find("get_raw_type_name<");
    if (start == std::string_view::npos) return "UnknownType";
    start += 18;
    auto end = p.rfind(">(");
    return p.substr(start, end - start);
#else
    return "UnknownType";
#endif
}

namespace detail {

constexpr bool starts_with_at(std::string_view s, std::size_t pos, std::string_view pre) {
    return pre.size() <= s.size() - pos && s.substr(pos, pre.size()) == pre;
}

constexpr bool is_ident_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') || c == '_';
}

// Normalizes a raw __PRETTY_FUNCTION__/__FUNCSIG__ type spelling so template
// headers print identically on GCC and Clang without per-type tags:
//   - strips the outer scope only (never cuts inside <...>),
//   - compresses std string spellings to std::string / std::string_view,
//   - drops std inline-namespace segments (__cxx11::, __1::),
//   - skips MSVC `class `/`struct ` argument prefixes (best effort),
//   - collapses "> >" to ">>".
// Anything unrecognized passes through untouched. Output never exceeds input
// length, so std::array<char, raw.size() + 1> always fits.
template <std::size_t Bound>
consteval std::array<char, Bound> normalize_type_name(std::string_view raw) {
    std::array<char, Bound> out{};
    std::size_t n = 0;
    char last = '\0';
    auto emit = [&](char c) {
        if (n + 1 <= Bound) {
            out[n++] = c;
            last = c;
        }
    };

    // Phase A: strip the outer scope (text before the first '<').
    std::size_t head_end = raw.find('<');
    if (head_end == std::string_view::npos) head_end = raw.size();
    std::size_t pos = 0;
    {
        std::string_view head = raw.substr(0, head_end);
        if (auto p = head.rfind("::"); p != std::string_view::npos) pos = p + 2;
    }
    const std::size_t start = pos;

    auto at_boundary = [&] {
        return pos == start || (!is_ident_char(raw[pos - 1]) && raw[pos - 1] != ':');
    };

    // Phase B: single forward streaming pass.
    while (pos < raw.size()) {
        // (1) MSVC `class `/`struct `/`union `/`enum ` argument prefixes.
        if (at_boundary()) {
            if (starts_with_at(raw, pos, "class ") || starts_with_at(raw, pos, "union ")) {
                pos += 6;
                continue;
            }
            if (starts_with_at(raw, pos, "struct ")) {
                pos += 7;
                continue;
            }
            if (starts_with_at(raw, pos, "enum ")) {
                pos += 5;
                continue;
            }
        }
        // (2) std string / string_view alias compression (any qualifier depth).
        if (at_boundary()) {
            std::string_view rep{};
            std::size_t adv = 0;
            auto try_pat = [&](std::string_view pat, std::string_view r) {
                if (adv == 0 && starts_with_at(raw, pos, pat)) {
                    adv = pat.size();
                    rep = r;
                }
            };
            try_pat("std::__cxx11::basic_string<char", "std::string");
            try_pat("std::__1::basic_string<char", "std::string");
            try_pat("std::basic_string<char", "std::string");
            try_pat("basic_string<char", "std::string");
            try_pat("std::__cxx11::basic_string_view<char", "std::string_view");
            try_pat("std::__1::basic_string_view<char", "std::string_view");
            try_pat("std::basic_string_view<char", "std::string_view");
            try_pat("basic_string_view<char", "std::string_view");
            if (adv > 0) {
                for (char c : rep) emit(c);
                pos += adv;
                // Skip the remainder of the argument list, if any: compilers
                // may spell either basic_string<char> or basic_string<char, ...>,
                // with an optional space before a nested closer (GCC style).
                std::size_t q = pos;
                while (q < raw.size() && raw[q] == ' ') q++;
                if (q < raw.size() && raw[q] == '>') {
                    pos = q + 1;
                    last = '>';
                } else if (q < raw.size() && raw[q] == ',') {
                    pos = q;
                    std::size_t depth = 1;
                    while (pos < raw.size() && depth > 0) {
                        char c = raw[pos++];
                        if (c == '<') depth++;
                        else if (c == '>') depth--;
                    }
                    last = '>';
                }
                continue;
            }
        }
        // (3) Drop std inline-namespace segments on other types.
        if ((starts_with_at(raw, pos, "__cxx11::") || starts_with_at(raw, pos, "__1::")) &&
            pos >= 5 && raw.substr(pos - 5, 5) == "std::") {
            pos += starts_with_at(raw, pos, "__cxx11::") ? 9 : 5;
            continue;
        }
        // (4) Collapse "> >" to ">>" (GCC nests with a space, Clang doesn't).
        if (raw[pos] == ' ' && last == '>') {
            std::size_t q = pos + 1;
            while (q < raw.size() && raw[q] == ' ') q++;
            if (q < raw.size() && raw[q] == '>') {
                pos = q;
                continue;
            }
        }
        // (5) Copy through.
        emit(raw[pos++]);
    }
    if (n < Bound) out[n] = '\0';
    return out;
}

template <typename T>
inline constexpr auto stored_normalized_type_name =
    normalize_type_name<get_raw_type_name<T>().size() + 1>(get_raw_type_name<T>());

} // namespace detail

constexpr std::string_view get_short_type_name(std::string_view full) {
    auto pos = full.rfind("::");
    if (pos == std::string_view::npos) return full;
    return full.substr(pos + 2);
}

template <typename T>
constexpr std::string_view resolve_type_name() {
    using CleanT = std::remove_cvref_t<T>;
    if constexpr (requires { { __debug_type_name_tag(static_cast<const CleanT*>(nullptr)) } -> std::convertible_to<std::string_view>; }) {
        return __debug_type_name_tag(static_cast<const CleanT*>(nullptr));
    } else {
        return std::string_view{detail::stored_normalized_type_name<CleanT>.data()};
    }
}

// ============================================================================
// Section 4: Type Traits and C++20 Concepts
// ============================================================================

struct DummyVisitor {
    template <typename V>
    void operator()(std::string_view, const V&) const {}
};

struct DebugAccess {
    template <typename T, typename Visitor>
    static void reflect(const T& obj, Visitor&& v) {
        if constexpr (requires { obj.__debug_reflect(v); }) {
            obj.__debug_reflect(std::forward<Visitor>(v));
        } else if constexpr (requires { __debug_reflect(obj, v); }) {
            __debug_reflect(obj, std::forward<Visitor>(v));
        }
    }
};

/**
 * @brief True when T has explicit reflection via DEBUG_FIELDS / DEBUG_STRUCT / DEBUG_AUTO.
 *
 * Satisfied by an intrusive `__debug_reflect` member or a non-intrusive
 * `__debug_reflect` overload. Explicit registration always wins over the
 * automatic aggregate fallback (see IsAutoReflectable).
 */
template <typename T>
concept IsDebugReflectable = requires(const std::remove_cvref_t<T>& t, DummyVisitor v) {
    t.__debug_reflect(v);
} || requires(const std::remove_cvref_t<T>& t, DummyVisitor v) {
    __debug_reflect(t, v);
};

// Container / Template traits
template <typename T>
struct is_optional_trait : std::false_type {};
template <typename T>
struct is_optional_trait<std::optional<T>> : std::true_type {};

template <typename T>
struct is_pair_trait : std::false_type {};
template <typename T, typename U>
struct is_pair_trait<std::pair<T, U>> : std::true_type {};

template <typename T>
struct is_tuple_trait : std::false_type {};
template <typename... Ts>
struct is_tuple_trait<std::tuple<Ts...>> : std::true_type {};

template <typename T>
struct is_unique_ptr_trait : std::false_type {};
template <typename T, typename D>
struct is_unique_ptr_trait<std::unique_ptr<T, D>> : std::true_type {};

template <typename T>
struct is_shared_ptr_trait : std::false_type {};
template <typename T>
struct is_shared_ptr_trait<std::shared_ptr<T>> : std::true_type {};

/**
 * @brief Matches `bool` (ignoring cv/ref). Prints as `true` / `false`.
 */
template <typename T>
concept IsBoolean = std::same_as<std::remove_cvref_t<T>, bool>;

/**
 * @brief Matches `char` (ignoring cv/ref). Prints single-quoted with escapes.
 */
template <typename T>
concept IsChar = std::same_as<std::remove_cvref_t<T>, char>;

/**
 * @brief Matches `std::string` / `std::string_view` (ignoring cv/ref).
 */
template <typename T>
concept IsStdString = std::same_as<std::remove_cvref_t<T>, std::string> ||
                      std::same_as<std::remove_cvref_t<T>, std::string_view>;

/**
 * @brief Matches C strings: `char` pointers and `char` arrays (any cv).
 *
 * Null pointers print as `nullptr`; otherwise the contents print quoted.
 */
template <typename T>
concept IsCString = (std::is_pointer_v<std::remove_cvref_t<T>> && 
                     std::same_as<std::remove_cv_t<std::remove_pointer_t<std::remove_cvref_t<T>>>, char>) ||
                    (std::is_array_v<std::remove_cvref_t<T>> && 
                     std::same_as<std::remove_cv_t<std::remove_extent_t<std::remove_cvref_t<T>>>, char>);

/**
 * @brief Matches any string-like type (IsStdString or IsCString).
 *
 * Prints double-quoted with escapes (see write_escaped_string).
 */
template <typename T>
concept IsStringLike = IsStdString<T> || IsCString<T>;

/**
 * @brief Matches `std::optional<T>`. Prints as `Some(v)` or `None`.
 */
template <typename T>
concept IsOptional = is_optional_trait<std::remove_cvref_t<T>>::value;

/**
 * @brief Matches `std::pair<T, U>`. Prints as `(first, second)`.
 */
template <typename T>
concept IsPair = is_pair_trait<std::remove_cvref_t<T>>::value;

/**
 * @brief Matches `std::tuple<Ts...>`. Prints as `(a, b, ...)` or `()` when empty.
 */
template <typename T>
concept IsTuple = is_tuple_trait<std::remove_cvref_t<T>>::value;

/**
 * @brief Matches `std::unique_ptr` / `std::shared_ptr`.
 *
 * Null prints as `nullptr`; otherwise the pointee is printed with cycle tracking.
 */
template <typename T>
concept IsSmartPointer = is_unique_ptr_trait<std::remove_cvref_t<T>>::value || 
                         is_shared_ptr_trait<std::remove_cvref_t<T>>::value;

/**
 * @brief Matches raw pointers other than C strings.
 *
 * Null prints as `nullptr`. `void*` and function pointers print as addresses;
 * other pointees are dereferenced with cycle tracking.
 */
template <typename T>
concept IsRawPointer = std::is_pointer_v<std::remove_cvref_t<T>> && !IsCString<T>;

/**
 * @brief Matches ranges with `begin`/`end`, excluding string-like types.
 *
 * Empty ranges print as `[]`; non-empty ranges print one element per line.
 */
template <typename T>
concept IsContainer = requires(const std::remove_cvref_t<T>& c) {
    std::begin(c);
    std::end(c);
} && !IsStringLike<T>;

/**
 * @brief Matches enum types.
 *
 * Registered enums (DEBUG_ENUM) print as `Type::Variant`; unregistered enums
 * print as `TypeName(value)` with the underlying value.
 */
template <typename T>
concept IsEnum = std::is_enum_v<std::remove_cvref_t<T>>;

/**
 * @brief Matches integral types except `bool` and `char`. Prints as a number.
 */
template <typename T>
concept IsIntegral = std::integral<std::remove_cvref_t<T>> && !IsBoolean<T> && !IsChar<T>;

/**
 * @brief Matches floating-point types. Prints as a number.
 */
template <typename T>
concept IsFloating = std::floating_point<std::remove_cvref_t<T>>;

/**
 * @brief True for public aggregates printable with no macro at all.
 *
 * Excludes unions, arrays, strings, optionals, pairs, tuples, smart/raw
 * pointers, containers, enums, and explicitly registered types (explicit
 * registration always wins). Auto discovery supports aggregates only with
 * up to 64 fields (see Section 4b); non-aggregates are never matched.
 */
template <typename T>
concept IsAutoReflectable =
    std::is_aggregate_v<std::remove_cvref_t<T>> &&
    !std::is_union_v<std::remove_cvref_t<T>> &&
    !std::is_array_v<std::remove_cvref_t<T>> &&
    !IsStringLike<T> && !IsOptional<T> && !IsPair<T> && !IsTuple<T> &&
    !IsSmartPointer<T> && !IsRawPointer<T> && !IsContainer<T> &&
    !IsEnum<T> && !IsDebugReflectable<T>;

/**
 * @brief Opt-in compile-time notice: true when T prints through the generic
 *        auto fallback (nested without explicit DEBUG).
 *
 * Use as `static_assert(!debug_derive::uses_generic_auto_v<MyType>)` to get a
 * compiler message naming each type that relies on automatic recursion, or
 * `static_assert(debug_derive::uses_generic_auto_v<T>)` to pin the fallback.
 * Unsupported types (non-aggregate, unregistered) instead print the runtime
 * marker `<unformattable TypeName>`, which likewise names the type.
 */
// ponytail: queryable trait, not a per-instantiation #pragma/[[deprecated]]
// hook — those either fire unconditionally or break -Wall -Wextra -pedantic.
template <typename T>
constexpr bool uses_generic_auto_v = IsAutoReflectable<std::remove_cvref_t<T>>;

// ============================================================================
// Section 4b: Automatic Aggregate Reflection (zero-argument discovery)
// ----------------------------------------------------------------------------
// Vendored PFR-lite. Field VALUES come from C++17 structured bindings;
// field NAMES come from the __PRETTY_FUNCTION__ non-type-template-parameter
// folding trick (the address of a structured-binding alias pretty-prints
// with the real member name on GCC and Clang). Technique inspired by
// Boost.PFR (Boost Software License 1.0); no external dependency.
// Limits: aggregates only, up to 64 fields. Anything else keeps using the
// explicit DEBUG_FIELDS(...) / DEBUG_STRUCT(...) / DEBUG_ENUM(...) macros.
// ============================================================================

namespace detail {
namespace auto_impl {

inline constexpr std::size_t AUTO_MAX_FIELDS = 64;

struct universal_type_auto {
    template <typename U>
    constexpr operator U() const noexcept;
};

template <std::size_t N>
struct auto_size_t : std::integral_constant<std::size_t, N> {};

template <typename T>
consteval std::size_t fields_count_auto() noexcept {
    using U = universal_type_auto;
    if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 64;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 63;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 62;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 61;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 60;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 59;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 58;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 57;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 56;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 55;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 54;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 53;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 52;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 51;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 50;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 49;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 48;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 47;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 46;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 45;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 44;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 43;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 42;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 41;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 40;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 39;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 38;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 37;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 36;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 35;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 34;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 33;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 32;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 31;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 30;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 29;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 28;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 27;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 26;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 25;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 24;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 23;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 22;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 21;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 20;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 19;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 18;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 17;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 16;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 15;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 14;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 13;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 12;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 11;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 10;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 9;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 8;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}, U{}}; }) return 7;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}, U{}}; }) return 6;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}, U{}}; }) return 5;
    else if constexpr (requires { T{U{}, U{}, U{}, U{}}; }) return 4;
    else if constexpr (requires { T{U{}, U{}, U{}}; }) return 3;
    else if constexpr (requires { T{U{}, U{}}; }) return 2;
    else if constexpr (requires { T{U{}}; }) return 1;
    else return 0;
}

// Tie overloads: one per arity (structured-binding count must be literal).
template <typename T>
constexpr auto tie_as_tuple_auto(T&, auto_size_t<0>) noexcept {
    return std::tuple<>{};
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<1>) noexcept {
    auto& [v0] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&>(v0);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<2>) noexcept {
    auto& [v0, v1] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&>(v0, v1);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<3>) noexcept {
    auto& [v0, v1, v2] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&>(v0, v1, v2);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<4>) noexcept {
    auto& [v0, v1, v2, v3] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&>(v0, v1, v2, v3);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<5>) noexcept {
    auto& [v0, v1, v2, v3, v4] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&>(v0, v1, v2, v3, v4);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<6>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&>(v0, v1, v2, v3, v4, v5);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<7>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&>(v0, v1, v2, v3, v4, v5, v6);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<8>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&>(v0, v1, v2, v3, v4, v5, v6, v7);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<9>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<10>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<11>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<12>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<13>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<14>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<15>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<16>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<17>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<18>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<19>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<20>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<21>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<22>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<23>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<24>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<25>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<26>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<27>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<28>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<29>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<30>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<31>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<32>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<33>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<34>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<35>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<36>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<37>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<38>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<39>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<40>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<41>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<42>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<43>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<44>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<45>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<46>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<47>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<48>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<49>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<50>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&, decltype(v49)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<51>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&, decltype(v49)&, decltype(v50)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<52>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&, decltype(v49)&, decltype(v50)&, decltype(v51)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<53>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&, decltype(v49)&, decltype(v50)&, decltype(v51)&, decltype(v52)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<54>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&, decltype(v49)&, decltype(v50)&, decltype(v51)&, decltype(v52)&, decltype(v53)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<55>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&, decltype(v49)&, decltype(v50)&, decltype(v51)&, decltype(v52)&, decltype(v53)&, decltype(v54)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<56>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&, decltype(v49)&, decltype(v50)&, decltype(v51)&, decltype(v52)&, decltype(v53)&, decltype(v54)&, decltype(v55)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<57>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&, decltype(v49)&, decltype(v50)&, decltype(v51)&, decltype(v52)&, decltype(v53)&, decltype(v54)&, decltype(v55)&, decltype(v56)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<58>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&, decltype(v49)&, decltype(v50)&, decltype(v51)&, decltype(v52)&, decltype(v53)&, decltype(v54)&, decltype(v55)&, decltype(v56)&, decltype(v57)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<59>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&, decltype(v49)&, decltype(v50)&, decltype(v51)&, decltype(v52)&, decltype(v53)&, decltype(v54)&, decltype(v55)&, decltype(v56)&, decltype(v57)&, decltype(v58)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<60>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&, decltype(v49)&, decltype(v50)&, decltype(v51)&, decltype(v52)&, decltype(v53)&, decltype(v54)&, decltype(v55)&, decltype(v56)&, decltype(v57)&, decltype(v58)&, decltype(v59)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<61>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&, decltype(v49)&, decltype(v50)&, decltype(v51)&, decltype(v52)&, decltype(v53)&, decltype(v54)&, decltype(v55)&, decltype(v56)&, decltype(v57)&, decltype(v58)&, decltype(v59)&, decltype(v60)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<62>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&, decltype(v49)&, decltype(v50)&, decltype(v51)&, decltype(v52)&, decltype(v53)&, decltype(v54)&, decltype(v55)&, decltype(v56)&, decltype(v57)&, decltype(v58)&, decltype(v59)&, decltype(v60)&, decltype(v61)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<63>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&, decltype(v49)&, decltype(v50)&, decltype(v51)&, decltype(v52)&, decltype(v53)&, decltype(v54)&, decltype(v55)&, decltype(v56)&, decltype(v57)&, decltype(v58)&, decltype(v59)&, decltype(v60)&, decltype(v61)&, decltype(v62)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62);
}

template <typename T>
constexpr auto tie_as_tuple_auto(T& val, auto_size_t<64>) noexcept {
    auto& [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62, v63] = const_cast<std::remove_cvref_t<T>&>(val);
    return std::tuple<decltype(v0)&, decltype(v1)&, decltype(v2)&, decltype(v3)&, decltype(v4)&, decltype(v5)&, decltype(v6)&, decltype(v7)&, decltype(v8)&, decltype(v9)&, decltype(v10)&, decltype(v11)&, decltype(v12)&, decltype(v13)&, decltype(v14)&, decltype(v15)&, decltype(v16)&, decltype(v17)&, decltype(v18)&, decltype(v19)&, decltype(v20)&, decltype(v21)&, decltype(v22)&, decltype(v23)&, decltype(v24)&, decltype(v25)&, decltype(v26)&, decltype(v27)&, decltype(v28)&, decltype(v29)&, decltype(v30)&, decltype(v31)&, decltype(v32)&, decltype(v33)&, decltype(v34)&, decltype(v35)&, decltype(v36)&, decltype(v37)&, decltype(v38)&, decltype(v39)&, decltype(v40)&, decltype(v41)&, decltype(v42)&, decltype(v43)&, decltype(v44)&, decltype(v45)&, decltype(v46)&, decltype(v47)&, decltype(v48)&, decltype(v49)&, decltype(v50)&, decltype(v51)&, decltype(v52)&, decltype(v53)&, decltype(v54)&, decltype(v55)&, decltype(v56)&, decltype(v57)&, decltype(v58)&, decltype(v59)&, decltype(v60)&, decltype(v61)&, decltype(v62)&, decltype(v63)&>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62, v63);
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-var-template"
#endif

template <class T>
struct auto_fake_wrapper {
    const T value;
};

// Never defined: used only inside unevaluated/consteval field-name probes.
// (If the linker ever asks for it, auto reflection was used at runtime.)
template <class T>
extern const auto_fake_wrapper<T> auto_fake_holder;

template <class T>
constexpr const T& fake_object_auto() noexcept {
    return auto_fake_holder<T>.value;
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

consteval std::string_view extract_auto_field_name(std::string_view pretty) noexcept {
    // GCC spells the probe "... auto ptr = (& fake_holder<T>...::value.T::field)]".
    // Clang spells it "... ptr = &fake_holder.value.field]]".
    // "= &" anchors both spellings.
    std::string_view s = pretty;
    constexpr std::string_view anchor = "= &";
    if (auto pos = s.find(anchor); pos != std::string_view::npos) {
        s.remove_prefix(pos + anchor.size());
    }
    while (!s.empty() && (s.front() == '(' || s.front() == ' ')) {
        s.remove_prefix(1);
    }
    while (!s.empty() && (s.back() == ']' || s.back() == ')')) {
        s.remove_suffix(1);
    }
    if (auto pos = s.rfind("->"); pos != std::string_view::npos) {
        return s.substr(pos + 2);
    }
    if (auto pos = s.rfind("::"); pos != std::string_view::npos) {
        return s.substr(pos + 2);
    }
    if (auto pos = s.rfind('.'); pos != std::string_view::npos) {
        return s.substr(pos + 1);
    }
    return s;
}

template <class Tag, auto Ptr>
consteval auto name_of_field_auto_impl() noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
    constexpr std::string_view sv = __FUNCSIG__;
#else
    constexpr std::string_view sv = __PRETTY_FUNCTION__;
#endif
    constexpr std::string_view name = extract_auto_field_name(sv);
    std::array<char, name.size() + 1> out{};
    for (std::size_t i = 0; i < name.size(); ++i) {
        out[i] = name[i];
    }
    return out;
}

template <class T, std::size_t I>
inline constexpr auto stored_auto_field_name = name_of_field_auto_impl<
    T,
    std::addressof(std::get<I>(tie_as_tuple_auto(
        const_cast<std::remove_cvref_t<T>&>(fake_object_auto<T>()),
        auto_size_t<fields_count_auto<T>()>{})))>();

template <class T, std::size_t I>
constexpr std::string_view auto_field_name() noexcept {
    return std::string_view{stored_auto_field_name<T, I>.data()};
}

template <class T, std::size_t... I>
constexpr auto auto_names_array(std::index_sequence<I...>) noexcept {
    return std::array<std::string_view, sizeof...(I)>{auto_field_name<T, I>()...};
}

template <typename Tuple, typename Visitor, std::size_t... I>
constexpr void visit_auto_with_index(Tuple& tup,
                                     const std::array<std::string_view, sizeof...(I)>& names,
                                     Visitor&& visitor,
                                     std::index_sequence<I...>) {
    (visitor(names[I], std::get<I>(tup)), ...);
}

template <typename T, typename Visitor>
constexpr void visit_auto(const T& obj, Visitor&& visitor) {
    using C = std::remove_cvref_t<T>;
    // Soft no-op for non-aggregates: explicit DEBUG_FIELDS(...) / DEBUG_STRUCT(...)
    // bodies always name this function (after an early `return;`), so it must
    // stay instantiable for e.g. classes with private members. The `if
    // constexpr` discards the structured-binding machinery in that case.
    if constexpr (std::is_aggregate_v<C>) {
        constexpr std::size_t N = fields_count_auto<C>();
        static_assert(N <= AUTO_MAX_FIELDS,
            "debug_derive: auto reflection supports up to 64 fields; "
            "list the fields explicitly with DEBUG_FIELDS(...) / DEBUG_STRUCT(...)");
        auto tup = tie_as_tuple_auto(const_cast<C&>(obj), auto_size_t<N>{});
        constexpr auto names = auto_names_array<C>(std::make_index_sequence<N>{});
        visit_auto_with_index(tup, names, std::forward<Visitor>(visitor),
                              std::make_index_sequence<N>{});
    }
}

} // namespace auto_impl
} // namespace detail

// ============================================================================
// Section 5: Traversal Context (Zero Dynamic Allocation, Cycle & Depth Limits)
// ============================================================================

/**
 * @brief Traversal state for one debug-print operation (no heap allocation).
 *
 * @param os Output stream receiving the formatted text.
 * @param indent_level Current nesting level (each level adds indent_size spaces).
 * @param indent_size Spaces per indentation level (default 2).
 * @param max_depth Maximum nesting depth (default 32); deeper nesting prints
 *        `<recursion limit reached>`.
 * @param current_depth Current nesting depth, incremented while recursing.
 *
 * Cycle detection uses a stack-allocated buffer of up to
 * MAX_TRACKED_ADDRESSES (128) visited addresses. Revisiting a tracked
 * address prints `<cycle: Type@addr>`. No dynamic memory is allocated.
 */
struct DebugContext {
    std::ostream& os;
    int indent_level = 0;
    int indent_size = 2;
    int max_depth = 32;
    int current_depth = 0;

    // Stack-allocated buffer for cycle detection: ZERO dynamic heap allocation!
    static constexpr size_t MAX_TRACKED_ADDRESSES = 128;
    const void* visited_addresses[MAX_TRACKED_ADDRESSES]{};
    size_t visited_count = 0;

    void write_indent() {
        int total = indent_level * indent_size;
        for (int i = 0; i < total; ++i) {
            os.put(' ');
        }
    }

    bool has_cycle(const void* ptr) const {
        for (size_t i = 0; i < visited_count; ++i) {
            if (visited_addresses[i] == ptr) return true;
        }
        return false;
    }

    bool push_address(const void* ptr) {
        if (visited_count < MAX_TRACKED_ADDRESSES) {
            visited_addresses[visited_count++] = ptr;
            return true;
        }
        return false;
    }

    void pop_address() {
        if (visited_count > 0) {
            --visited_count;
        }
    }
};

/**
 * @brief RAII guard that tracks one address for cycle detection.
 *
 * Pushes `ptr` on construction (unless null) and pops it on destruction,
 * so sibling subtrees do not falsely report cycles.
 */
struct AddressTrackerGuard {
    DebugContext& ctx;
    bool pushed = false;

    AddressTrackerGuard(DebugContext& c, const void* ptr) : ctx(c) {
        if (ptr != nullptr) {
            pushed = ctx.push_address(ptr);
        }
    }

    ~AddressTrackerGuard() {
        if (pushed) {
            ctx.pop_address();
        }
    }
};

// ============================================================================
// Section 6: Character and String Escaping (Writing Directly to ostream)
// ============================================================================

/**
 * @brief Writes a string double-quoted with escapes (`\"`, `\\`, `\n`, `\r`,
 *        `\t`, `\0`, and `\xNN` for other control characters).
 */
inline void write_escaped_string(std::ostream& os, std::string_view s) {
    os.put('"');
    for (char c : s) {
        switch (c) {
            case '"':  os << "\\\""; break;
            case '\\': os << "\\\\"; break;
            case '\n': os << "\\n";  break;
            case '\r': os << "\\r";  break;
            case '\t': os << "\\t";  break;
            case '\0': os << "\\0";  break;
            default:
                if (static_cast<unsigned char>(c) < 32) {
                    char buf[5];
                    std::snprintf(buf, sizeof(buf), "\\x%02x", static_cast<unsigned char>(c));
                    os << buf;
                } else {
                    os.put(c);
                }
                break;
        }
    }
    os.put('"');
}

/**
 * @brief Writes a character single-quoted with escapes (`\'`, `\\`, `\n`,
 *        `\r`, `\t`, `\0`, and `\xNN` for other control characters).
 */
inline void write_escaped_char(std::ostream& os, char c) {
    os.put('\'');
    switch (c) {
        case '\'': os << "\\'"; break;
        case '\\': os << "\\\\"; break;
        case '\n': os << "\\n";  break;
        case '\r': os << "\\r";  break;
        case '\t': os << "\\t";  break;
        case '\0': os << "\\0";  break;
        default:
            if (static_cast<unsigned char>(c) < 32) {
                char buf[5];
                std::snprintf(buf, sizeof(buf), "\\x%02x", static_cast<unsigned char>(c));
                os << buf;
            } else {
                os.put(c);
            }
            break;
    }
    os.put('\'');
}

// ============================================================================
// Section 7: Recursive Debug Print Engine
// ============================================================================

template <typename T>
void debug_print_value(const T& val, DebugContext& ctx);

template <typename Tuple, size_t... Is>
void format_tuple_impl(const Tuple& tup, DebugContext& ctx, std::index_sequence<Is...>) {
    ctx.os.put('(');
    if constexpr (sizeof...(Is) > 0) {
        size_t idx = 0;
        auto print_elem = [&](const auto& elem) {
            if (idx++ > 0) ctx.os << ", ";
            debug_print_value(elem, ctx);
        };
        (print_elem(std::get<Is>(tup)), ...);
    }
    ctx.os.put(')');
}

/**
 * @brief Recursive print engine: dispatches on concepts (scalars, strings,
 *        optionals, pointers, pairs/tuples, reflected structs, containers,
 *        enums) in precedence order.
 *
 * Explicit reflection (IsDebugReflectable) is tried before automatic
 * aggregate reflection (IsAutoReflectable), so nested types recurse with no
 * per-type code: with-DEBUG nests print byte-identical to standalone, bare
 * aggregates recurse generically, and the single generic path reuses the
 * depth-32 / cycle-128 guards with const/ref/value preserved. Compile-time
 * notice for the auto path: `uses_generic_auto_v<T>`. Fallback: types with
 * an `operator<<` print via that operator; otherwise prints
 * `<unformattable TypeName>`.
 */
template <typename T>
void debug_print_value(const T& val, DebugContext& ctx) {
    using CleanT = std::remove_cvref_t<T>;

    if (ctx.current_depth > ctx.max_depth) {
        ctx.os << "<recursion limit reached>";
        return;
    }

    if constexpr (IsBoolean<CleanT>) {
        ctx.os << (val ? "true" : "false");
    } else if constexpr (IsChar<CleanT>) {
        write_escaped_char(ctx.os, val);
    } else if constexpr (std::same_as<CleanT, signed char> || std::same_as<CleanT, unsigned char>) {
        ctx.os << static_cast<int>(val);
    } else if constexpr (IsIntegral<CleanT> || IsFloating<CleanT>) {
        ctx.os << val;
    } else if constexpr (IsStringLike<CleanT>) {
        if constexpr (std::is_pointer_v<CleanT>) {
            if (val == nullptr) {
                ctx.os << "nullptr";
                return;
            }
        }
        write_escaped_string(ctx.os, std::string_view(val));
    } else if constexpr (IsOptional<CleanT>) {
        if (val.has_value()) {
            ctx.os << "Some(";
            ctx.current_depth++;
            debug_print_value(*val, ctx);
            ctx.current_depth--;
            ctx.os.put(')');
        } else {
            ctx.os << "None";
        }
    } else if constexpr (IsSmartPointer<CleanT>) {
        if (!val) {
            ctx.os << "nullptr";
            return;
        }
        using Pointee = typename CleanT::element_type;
        const void* raw = static_cast<const void*>(val.get());
        if (ctx.has_cycle(raw)) {
            ctx.os << "<cycle: " << resolve_type_name<Pointee>() << "@" << raw << ">";
            return;
        }
        AddressTrackerGuard guard(ctx, raw);
        ctx.current_depth++;
        debug_print_value(*val, ctx);
        ctx.current_depth--;
    } else if constexpr (IsRawPointer<CleanT>) {
        if (val == nullptr) {
            ctx.os << "nullptr";
            return;
        }
        using Pointee = std::remove_pointer_t<CleanT>;
        if constexpr (std::is_void_v<Pointee> || std::is_function_v<Pointee>) {
            // Undereferenceable: no value to write, keep the address.
            ctx.os << reinterpret_cast<const void*>(val);
        } else {
            const void* raw = static_cast<const void*>(val);
            if (ctx.has_cycle(raw)) {
                ctx.os << "<cycle: " << resolve_type_name<Pointee>() << "@" << raw << ">";
                return;
            }
            AddressTrackerGuard guard(ctx, raw);
            ctx.current_depth++;
            debug_print_value(*val, ctx);
            ctx.current_depth--;
        }
    } else if constexpr (IsPair<CleanT>) {
        ctx.os.put('(');
        ctx.current_depth++;
        debug_print_value(val.first, ctx);
        ctx.os << ", ";
        debug_print_value(val.second, ctx);
        ctx.current_depth--;
        ctx.os.put(')');
    } else if constexpr (IsTuple<CleanT>) {
        ctx.current_depth++;
        format_tuple_impl(val, ctx, std::make_index_sequence<std::tuple_size_v<CleanT>>{});
        ctx.current_depth--;
    } else if constexpr (IsDebugReflectable<CleanT>) {
        ctx.os << resolve_type_name<CleanT>() << " {";
        bool has_fields = false;
        ctx.indent_level++;
        ctx.current_depth++;

        DebugAccess::reflect(val, [&](std::string_view field_name, const auto& field_val) {
            if (has_fields) {
                ctx.os << ",\n";
            } else {
                ctx.os.put('\n');
                has_fields = true;
            }
            ctx.write_indent();
            ctx.os << field_name << ": ";
            debug_print_value(field_val, ctx);
        });

        ctx.current_depth--;
        ctx.indent_level--;
        if (has_fields) {
            ctx.os.put('\n');
            ctx.write_indent();
        }
        ctx.os.put('}');
    } else if constexpr (IsAutoReflectable<CleanT>) {
        ctx.os << resolve_type_name<CleanT>() << " {";
        bool has_fields = false;
        ctx.indent_level++;
        ctx.current_depth++;

        detail::auto_impl::visit_auto(val, [&](std::string_view field_name, const auto& field_val) {
            if (has_fields) {
                ctx.os << ",\n";
            } else {
                ctx.os.put('\n');
                has_fields = true;
            }
            ctx.write_indent();
            ctx.os << field_name << ": ";
            debug_print_value(field_val, ctx);
        });

        ctx.current_depth--;
        ctx.indent_level--;
        if (has_fields) {
            ctx.os.put('\n');
            ctx.write_indent();
        }
        ctx.os.put('}');
    } else if constexpr (IsContainer<CleanT>) {
        auto it = std::begin(val);
        auto end = std::end(val);
        if (it == end) {
            ctx.os << "[]";
            return;
        }
        ctx.os << "[\n";
        ctx.indent_level++;
        ctx.current_depth++;
        bool first = true;
        for (; it != end; ++it) {
            if (!first) {
                ctx.os << ",\n";
            } else {
                first = false;
            }
            ctx.write_indent();
            debug_print_value(*it, ctx);
        }
        ctx.current_depth--;
        ctx.indent_level--;
        ctx.os.put('\n');
        ctx.write_indent();
        ctx.os.put(']');
    } else if constexpr (IsEnum<CleanT>) {
        if constexpr (requires { { __debug_enum_to_string(val) } -> std::convertible_to<std::string_view>; }) {
            std::string_view s = __debug_enum_to_string(val);
            if (!s.empty()) {
                ctx.os << s;
                return;
            }
        }
        ctx.os << resolve_type_name<CleanT>() << "(" 
               << static_cast<std::underlying_type_t<CleanT>>(val) << ")";
    } else {
        // Engine fallback: streamable types use operator<<, anything else
        // prints as <unformattable TypeName>.
        if constexpr (requires(std::ostream& s, const CleanT& v) { s << v; }) {
            ctx.os << val;
        } else {
            ctx.os << "<unformattable " << resolve_type_name<CleanT>() << ">";
        }
    }
}

// ============================================================================
// Section 8: Public User APIs
// ============================================================================

/**
 * @brief Pretty debug-print any supported object directly to an ostream.
 *
 * @param val The object to inspect.
 * @param os Output stream (defaults to std::cout).
 * @param indent_size Spaces per indentation level (defaults to 2).
 */
template <typename T>
void debug_print(const T& val, std::ostream& os = std::cout, int indent_size = 2) {
    DebugContext ctx{os, 0, indent_size, 32, 0};
    // Seed cycle detection with root address if passed by reference/pointer
    ctx.push_address(static_cast<const void*>(&val));
    debug_print_value(val, ctx);
    os.put('\n');
}

/**
 * @brief Format any supported object to a std::string.
 *
 * @param val The object to inspect.
 * @param indent_size Spaces per indentation level (defaults to 2).
 * @return std::string Formatted representation.
 */
template <typename T>
std::string to_debug_string(const T& val, int indent_size = 2) {
    std::ostringstream oss;
    DebugContext ctx{oss, 0, indent_size, 32, 0};
    // Seed cycle detection with root address if passed by reference/pointer
    ctx.push_address(static_cast<const void*>(&val));
    debug_print_value(val, ctx);
    return oss.str();
}

} // namespace debug_derive

// Bring primary functions into global or user namespace for convenience
using debug_derive::debug_print;
using debug_derive::to_debug_string;

// ============================================================================
// Section 9: Standard-version separation (Version B C++20 vs Version A C++23)
// ----------------------------------------------------------------------------
// This is ONE header for both versions; the versions differ only by the
// `-std=` flag and compiler floor, not by code path:
//   - Version B (canonical): compiled `-std=c++20` (GCC 13 / Clang 18 floor).
//   - Version A: the same header compiled `-std=c++23`.
// C++23 provides no data-member reflection: P2996 (reflexpr / std::meta) is
// C++26, so no new introspection is used or gated here. Private-member
// printing stays on the explicit `DEBUG_FIELDS(a, b, q, c)` friend path in
// both versions; zero-arg auto-discovery remains aggregates-only.
// ============================================================================
