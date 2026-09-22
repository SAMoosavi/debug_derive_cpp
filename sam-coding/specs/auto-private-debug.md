# auto-private-debug
## 1 Goal — print private/protected members via intrusive macro, recursive, release-safe.
## 2 Scope in: DEBUG_FIELDS(a,b,q,c) canonical (friend DebugAccess); multi-section+primitives; NDEBUG no-op preserved. Out: true zero-arg auto-naming of privates (impossible std C++20/23); no P2996/C++26; no external libs. SHALL NOT use silent UB in default path.
## 3 Stories — When class A{a,b(prot),q(priv),c + DEBUG_FIELDS(a,b,q,c)} prints, system SHALL emit all 4 name:value pairs recursively. When zero-arg DEBUG_FIELDS() on non-aggregate A, system SHALL print `A {}` (documented fallback).
## 4 Acceptance — Given A{1,2,3,4}/When to_debug_string/Then output contains a:1,b:2,q:3,c:4. Given NDEBUG/When compiled/Then output identical + zero overhead (<1% bench). Given GCC13+Clang18/When tests run/Then all pass.
## 5 Edge cases + constraints — unions/bitfields/virtuals excluded; >64 fields fails compile; depth>32 → `<recursion limit reached>`; cycles → `<cycle: T@addr>`; UB variant (#define private public) opt-in only, quarantined test file, never default.
## 6 Open questions — None (decided: DEBUG()=zero-arg DEBUG_FIELDS(); C++23 adds no introspection vs C++20, sections differ only by std flag/compiler floor).
## 7 Files + Tasks — include/debug_derive.hpp §2/§4 (friend path already works; add static_assert+docs) [P]; tests/test_private.cpp: A multi-section + nested-private + NDEBUG test [P]; tests/test_types.hpp: add fixture A; README: private-canonical + C++20/23 note. C++23 file = same header @ -std=c++23; C++20 = -std=c++20.
