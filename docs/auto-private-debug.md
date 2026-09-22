# auto-private-debug

Print private/protected members via the intrusive macro. Recursive,
release-safe (`NDEBUG` is a no-op: output identical, zero overhead).

## Usage (canonical)

```cpp
#include "debug_derive.hpp"

class A {
    int a;          // private-implicit
protected:
    int b;
private:
    int q;
public:
    int c;
    A(int a_, int b_, int q_, int c_) : a(a_), b(b_), q(q_), c(c_) {}
    DEBUG_FIELDS(a, b, q, c)   // befriends debug_derive::DebugAccess
};

A v{1, 2, 3, 4};
debug_print(v);   // A {\n  a: 1,\n  b: 2,\n  q: 3,\n  c: 4\n}
```

The explicit field list reaches every access section through the
`DebugAccess` friend path — no engine change was needed. Zero-arg
`DEBUG_FIELDS()` on a non-aggregate is a documented no-op fallback that
prints `Type {}`; list fields explicitly instead.

## C++20 vs C++23 separation

One header, two standards — the versions differ only by the `-std=` flag
and compiler floor, never by code path:

| Version | Compiled as | Notes |
|---|---|---|
| B (canonical) | `-std=c++20` (GCC 13 / Clang 18 floor) | All behavior specified here |
| A | `-std=c++23`, same header | No new introspection |

C++23 provides no data-member reflection over C++20: P2996
(`reflexpr` / `std::meta`) is C++26, so it is explicitly out of scope and
no C++26 code is used. Auto-discovery stays aggregates-only in both
versions (no private members, no user constructors, no virtuals/bitfields,
no unions; up to 64 fields).

## Limits (by design)

- Max 64 fields per macro (`CPPDERIVE_PP_MAP_0..64`); more fails to compile.
- Nesting depth > 32 prints `<recursion limit reached>`.
- Revisited addresses print `<cycle: Type@addr>` (128-entry stack buffer).

## UB quarantine note

`#define private public` before including headers is undefined behavior and
is NEVER part of the default path. It exists only as an opt-in variant,
quarantined to its own translation unit, and must never be enabled by
default or leak into the library, tests, or build flags.

## Debug-auto via quarantined TU (opt-in, UB explicitly marked)

`tests/test_debug_auto_private.cpp` (executable `debug_auto_private`, CTest
`AutoPrivateDebug`) shows zero-arg `DEBUG_FIELDS()` auto-printing
private/protected members in Debug builds only. Technique (UB — confined to
that one TU, never a pattern to copy):

1. Include everything first: stdlib, GTest, then `debug_derive.hpp` — so no
   library or standard header is ever parsed under the hack (no stdlib
   poisoning).
2. `#ifndef NDEBUG` only: `#pragma push_macro` + `#define private public` /
   `#define protected public`.
3. Define pure-data demo types (`DebugAutoA`, `DebugAutoMulti`): NO user
   constructor, so with the hack they become aggregates and the Section 4b
   engine discovers all members with real names.
4. `#pragma pop_macro` immediately after; TESTs assert the full `a/b/q/c`
   output in Debug and the `Type {}` fallback in Release.

Limits:

- Automatic covers aggregate-eligible types only. Types with a user
  constructor (like fixture `A` in `tests/test_types.hpp`) stay
  non-aggregate even under the hack — keep the canonical explicit
  `DEBUG_FIELDS(a, b, q, c)` for those.
- Debug-only: Release (`NDEBUG`) compiles identical to today — the hack is
  off and zero-arg on such types prints `Type {}`.
- The hack TU is standalone: its types are defined locally (no ODR impact),
  and no `-Dprivate=public` flag exists anywhere in the build.
