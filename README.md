# CppDebugDerive

A header-only C++20 pretty-printer in the style of Rust's `#[derive(Debug)]`.
Include one header, register your types with a macro, then print them with
`debug_print` or format them with `to_debug_string`.

```cpp
#include "debug_derive.hpp"

struct Point { int x; int y; DEBUG_FIELDS(x, y) };

Point p{1, 2};
debug_print(p);              // Point {\n  x: 1,\n  y: 2\n}
std::string s = to_debug_string(p);
```

- Library version: `0.1.0` (`DEBUG_DERIVE_VERSION_MAJOR/MINOR/PATCH` and
  `DEBUG_DERIVE_VERSION_STRING` in `include/debug_derive.hpp`).
- No external dependencies; the CMake build only needs GoogleTest to compile
  the test suite (fetched automatically if not installed).

## Features

- **Intrusive registration** — `DEBUG_FIELDS(a, b)` inside the class body.
  Works at any access level (it befriends `debug_derive::DebugAccess`) and
  preserves aggregate initialization.
- **Non-intrusive registration** — `DEBUG_STRUCT(Type, x, y)` at global scope
  for types you cannot edit. `DEBUG_ALLOW_EXTERNAL_FRIEND(Type)` inside the
  class grants it access to private members.
- **Auto-discovery for aggregates** — `DEBUG_FIELDS()` (intrusive),
  `DEBUG_STRUCT(Type)` / `DEBUG_AUTO(Type)` (non-intrusive), or no macro at
  all for public aggregates. Field names are recovered automatically; an
  explicit field list always wins over auto-discovery.
- **Enum support** — `DEBUG_ENUM(Type, A, B)` prints `Type::Variant`;
  unregistered enums fall back to `TypeName(value)`.
- **Broad type coverage** — nested structs, STL containers, `std::map` (as a
  list of `(key, value)` pairs), `std::pair`/tuples, `std::optional`
  (`Some(v)` / `None`), raw/smart pointers, strings with C-style escaping,
  and templates (including partial specializations).
- **Safety guards** — cycle detection (`<cycle: Type@addr>`) and a nesting
  depth limit (`<recursion limit reached>`), with zero heap allocation during
  traversal.
- **Tested** — 33 GoogleTest tests, all in the `DebugDeriveTest` suite.

## Requirements

- C++20 (`__VA_OPT__`, concepts, `std::remove_cvref_t`; the header errors out
  below `__cplusplus >= 202002L`).
- GCC 13 or Clang 18 (see [Compiler support](#compiler-support)).
- CMake 3.20+ to build the demo/tests (`cmake_minimum_required(VERSION 3.20)`).
- GoogleTest v1.14.0 for the test suite: `find_package(GTest)` is tried first,
  otherwise it is fetched via `FetchContent` (needs network on first
  configure). Compiler flags are `-Wall -Wextra -pedantic` (`/W4
  /permissive-` on MSVC).

## Quickstart

### 1. Intrusive struct (explicit field list)

```cpp
#include "debug_derive.hpp"

struct Address {
    std::string city;
    int zip;

    DEBUG_FIELDS(city, zip)
};

struct User {
    std::string name;
    int age;
    Address address;

    DEBUG_FIELDS(name, age, address)
};

User user{"Alice", 30, {"Amsterdam", 1011}};
debug_print(user);
// User {
//   name: "Alice",
//   age: 30,
//   address: Address {
//     city: "Amsterdam",
//     zip: 1011
//   }
// }
```

`DEBUG_FIELDS(...)` goes inside the class body and may list private members
too. `debug_print(val, os = std::cout, indent_size = 2)` writes to a stream;
`to_debug_string(val, indent_size = 2)` returns a `std::string`. Both are
brought to global scope by `using` declarations in the header.

### 2. Auto-discovery (no field list)

For public aggregates you can skip the field list. All three forms below
print real member names with no extra dependency:

```cpp
#include "debug_derive.hpp"

// a) Intrusive, zero arguments:
struct AutoAddress {
    std::string city;
    int zip;
    DEBUG_FIELDS()
};

// b) Non-intrusive, at global scope (for third-party types):
struct ThirdPartyPoint { int x; int y; };
DEBUG_AUTO(ThirdPartyPoint)   // equivalent to DEBUG_STRUCT(ThirdPartyPoint)

// c) No macro at all: any public aggregate is discovered by the engine.
struct BarePoint { int x; int y; };
```

```cpp
ThirdPartyPoint p{10, 20};
debug_print(p);   // ThirdPartyPoint {\n  x: 10,\n  y: 20\n}
```

Explicit listing always wins: `DEBUG_FIELDS(a, b)` on a type with members
declared `b, a` prints `a` first. A zero-arg macro on a non-aggregate
compiles but prints `Type {}` — list its fields explicitly instead.

### 3. Enums (opt-in)

```cpp
#include "debug_derive.hpp"

enum class ServerState { Starting, Running, Stopped };
DEBUG_ENUM(ServerState, Starting, Running, Stopped)   // global scope

enum Priority { Low, Medium, High };                  // unregistered

debug_print(ServerState::Running);  // ServerState::Running
debug_print(High);                  // Priority(2)
```

Registered enumerators print as `Type::Variant`. Unregistered enums print as
`TypeName(underlying_value)`.

## Using it as a dependency

The library is the `debug_derive` `INTERFACE` target (C++20 via
`target_compile_features`, headers in `include/`).

**Option A — `add_subdirectory`:**

```cmake
add_subdirectory(CppDebugDerive)   # wherever you vendor this repo
target_link_libraries(myapp PRIVATE debug_derive)
```

```cpp
#include <debug_derive.hpp>
```

**Option B — install, then `find_package`:**

The build installs the header plus a CMake package config
(`CppDebugDeriveConfig.cmake` / `CppDebugDeriveTargets.cmake`, namespace
`CppDebugDerive::`):

```bash
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build build
cmake --install build
```

```cmake
find_package(CppDebugDerive REQUIRED)
target_link_libraries(myapp PRIVATE CppDebugDerive::debug_derive)
```

**Configure options** (both `ON` by default):

| Option | Effect |
|---|---|
| `BUILD_DEMO` | Builds the `demo` executable from `examples/demo.cpp` |
| `BUILD_TESTING` | Builds the `run_tests` executable and registers the `AllTests` CTest |

## Build, test, demo

```bash
cmake -S . -B build
cmake --build build
./build/run_tests                                            # full GTest suite
ctest --test-dir build                                       # same via CTest (AllTests)
./build/run_tests --gtest_filter='DebugDeriveTest.CycleDetection'  # single test
./build/demo                                                 # behavior demo
```

There are also `gcc-debug` and `clang-debug` presets in `CMakePresets.json`
(GCC 13 / Clang 18, Debug, demo + testing on):

```bash
cmake --preset gcc-debug && cmake --build build/gcc-debug
cmake --preset clang-debug && cmake --build build/clang-debug
ctest --preset gcc-debug
```

Single-file probes without CMake:

```bash
g++ -std=c++20 -Wall -Wextra -pedantic <file>.cpp -o /tmp/<name> && /tmp/<name>
clang++ -std=c++20 -Wall -Wextra -pedantic <file>.cpp -o /tmp/<name> && /tmp/<name>
```

## Test layout

`tests/CMakeLists.txt` builds `run_tests` from the files below (linked
against `debug_derive` and `GTest::gtest_main`, registered as CTest
`AllTests`). Shared fixtures live in `tests/test_types.hpp`. All 33 tests run
under the `DebugDeriveTest` suite.

| File | Covers (`TEST` names) |
|---|---|
| `test_primitives.cpp` | `PrimitiveValues` — bool, ints, floats, chars, escape chars, `uint8_t` as number |
| `test_strings.cpp` | `StringsAndEscaping` — `std::string`, C-strings, `string_view`, null C-string, escapes |
| `test_enums.cpp` | `Enums` — registered `Type::Variant` vs unregistered `TypeName(value)` |
| `test_structs.cpp` | `EmptyStructs`, `NestedStructs`, `DeeplyNestedStructures`, `PrivateMembers` |
| `test_containers.cpp` | `ContainersAndVectors`, `MAP`, `Optionals`, `PairsAndTuples` |
| `test_pointers.cpp` | `RawPointerNullAndScalars`, `RawPointerDerefValues`, `RawPointerUndereferenceable`, `SmartPointerNullAndValue` |
| `test_auto.cpp` | `AutoIntrusive`, `AutoNonIntrusive`, `AutoWithoutAnyMacro`, `AutoEmpty`, `AutoNestedContainers`, `ExplicitRegistrationTakesPrecedence`, `NestedPrimitiveMember`, `NestedWithExplicitDebug`, `NestedWithoutDebug`, `ThreeLevelNesting`, `MixedPrimitiveAndUserDefined` |
| `test_templates.cpp` | `TemplateContainerPrimary`, `TemplateContainerStringSpec`, `TemplateContainerIntSpec`, `TemplateContainerNested`, `TemplateContainerSimplePoint` |
| `test_safety.cpp` | `CycleDetection`, `DepthLimiting` |

## Formatting rules and limits

**Structs:** `Type {` on one line, one `name: value` per line indented 2
spaces per level (configurable via the `indent_size` parameter), entries
separated by `,`, closing `}` dedented. Types with no visible fields print as
`Type {}` (e.g. `EmptyStruct {}`).

**Containers:** empty prints as `[]`; non-empty prints one element per line
inside `[...]`. `std::map` prints as a list of `(key, value)` pairs.

**Pairs/tuples:** `(first, second)`; an empty tuple prints as `()`.

**Optionals:** `Some(value)` or `None`.

**Strings/chars:** strings print double-quoted with `\"`, `\\`, `\n`, `\r`,
`\t`, `\0` escapes (`\xNN` for other control characters); chars print
single-quoted with the same escapes (`'Z'`, `'\n'`). Applies to
`std::string`, `string_view`, and C-strings alike.

**Scalars:** `true`/`false` for bools; numbers via `operator<<`
(`signed`/`unsigned char` print as numbers, e.g. `uint8_t(65)` → `65`).

**Enums:** registered → `Type::Variant` (e.g. `TestColor::Green`);
unregistered → `TypeName(underlying_value)` (e.g. `TestDirection(0)`).

**Pointers:**
- Null (raw, smart, or null C-string) prints as `nullptr`.
- Dereferenceable raw/smart pointers print the pointee (`int*` to `42` prints
  `42`).
- `void*` and function pointers cannot be dereferenced and print as addresses
  (same text as streaming the `const void*`).
- Revisiting a tracked address prints `<cycle: Type@addr>`.

**Fallbacks:** types with `operator<<` print through it; anything else prints
`<unformattable TypeName>`. Explicit reflection is tried before automatic
aggregate reflection, and bare aggregates nested in explicit parents recurse
generically with identical formatting. `uses_generic_auto_v<T>` is an opt-in
compile-time notice naming types that rely on the generic auto path
(see `docs/automatic-nested.md`).

**Hard limits (by design):**

| Limit | Value | Behavior past the limit |
|---|---|---|
| Fields per macro | 64 (`CPPDERIVE_PP_MAP_0..64`) | Fails to compile |
| Max nesting depth (`DebugContext::max_depth`) | 32 | Prints `<recursion limit reached>` |
| Tracked cycle addresses (`MAX_TRACKED_ADDRESSES`) | 128, stack-allocated | Further addresses are not tracked |
| Auto-discovery | Aggregates only (no private members, no user constructors, no virtuals/bitfields, no unions), up to 64 fields | Non-aggregate with zero args prints `Type {}`; list fields explicitly |

## Compiler support

- Supported toolchains: **GCC 13** and **Clang 18** (both exercised by
  `CMakePresets.json` as `gcc-debug` / `clang-debug`). Behavior-changing edits
  should be verified under both compilers with the single-file probe commands
  above.
- Type names are read from `__PRETTY_FUNCTION__` (separate `__clang__` /
  `__GNUC__` branches; an `_MSC_VER`/`__FUNCSIG__` branch also exists) and
  unified by `detail::normalize_type_name`: outer scopes stripped,
  `basic_string<char, …>` → `std::string` (likewise `string_view`),
  `> >` → `>>`. Template headers such as `Container<std::string, int>` therefore
  render identically on GCC and Clang, and the tests need no per-compiler
  expectations. Unknown spellings pass through untouched.

## Project layout

```
.
├── AGENTS.md                  # contributor notes (build commands, macro rules, limits)
├── CMakeLists.txt             # debug_derive INTERFACE target, demo/tests, install + package config
├── CMakePresets.json          # gcc-debug / clang-debug presets
├── include/
│   └── debug_derive.hpp       # the entire library (~1590 lines, sections 1–8)
├── examples/
│   └── demo.cpp               # behavior demo (builds to ./build/demo)
└── tests/
    ├── CMakeLists.txt         # run_tests target + AllTests CTest
    ├── test_types.hpp         # shared fixtures (structs, enums, templates, auto types)
    ├── test_primitives.cpp
    ├── test_strings.cpp
    ├── test_enums.cpp
    ├── test_structs.cpp
    ├── test_containers.cpp
    ├── test_pointers.cpp
    ├── test_auto.cpp
    ├── test_templates.cpp
    └── test_safety.cpp
```

## License

No `LICENSE` file exists in the repository yet.
