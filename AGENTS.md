# AGENTS.md

Header-only C++20 `#[derive(Debug)]` clone. Single library file: `include/debug_derive.hpp` (self-contained). `examples/demo.cpp` = demo, `tests/test_*.cpp` = GTest suite (one `run_tests` binary), `CMakeLists.txt` + `CMakePresets.json` = build definition. `README.md` = user docs.

## Build / test (exact commands)

```bash
cmake -S . -B build
cmake --build build
./build/run_tests                 # full GTest suite (28 tests)
ctest --test-dir build            # same via CTest (`AllTests`)
./build/run_tests --gtest_filter='DebugDeriveTest.CycleDetection'  # single test
./build/demo                      # behavior demo
```

- Requires C++20 (`__VA_OPT__`, concepts, `remove_cvref_t`). Toolchain here: GCC 13, Clang 18, CMake 3.28.
- Flags are `-Wall -Wextra -pedantic` (per-target); keep new code warning-clean.
- Targets: `debug_derive` (INTERFACE lib), `demo`, `run_tests`. Options: `BUILD_DEMO`, `BUILD_TESTING`. Installs header + `CppDebugDerive::` package config.
- First CMake configure fetches GTest v1.14.0 via FetchContent — needs network. `build/` is already configured so prefer rebuilding over reconfiguring.
- Single-file probes need `-Iinclude`: `g++ -std=c++20 -Wall -Wextra -pedantic -Iinclude <file>.cpp -o /tmp/<name> && /tmp/<name>` (same with `clang++`). Do not leave compiled binaries in repo root.

## Dual-compiler quirk

`get_raw_type_name()` in `include/debug_derive.hpp` parses `__PRETTY_FUNCTION__`/`__FUNCSIG__` with separate `__clang__` / `__GNUC__` / `_MSC_VER` branches, and `detail::normalize_type_name` (Section 3) unifies the result: outer scopes stripped, `basic_string<char,…>` → `std::string` (same for `string_view`), `> >` → `>>`. Template headers like `Container<std::string, int>` are therefore identical on GCC and Clang — no per-compiler expectations needed. Unknown spellings pass through untouched. Verify behavior-changing edits under both:

```bash
g++ -std=c++20 -Iinclude -Ibuild/_deps/googletest-src/googletest/include tests/test_types.hpp -fsyntax-only
clang++ -std=c++20 -Wall -Wextra -pedantic -Iinclude -Itests -Ibuild/_deps/googletest-src/googletest/include tests/test_pointers.cpp build/lib/libgtest.a build/lib/libgtest_main.a -o /tmp/probe && /tmp/probe
```

## Source boundaries

- `include/debug_derive.hpp` is the only file to edit for library changes. Sections 1–8 (+4b auto engine) are marked in-file. Version: `DEBUG_DERIVE_VERSION_*` (currently 0.1.0); file starts with a C++20 `#error` guard.
- `tests/test_types.hpp` holds all shared test types; `tests/test_<area>.cpp` holds the `TESTs` (suite `DebugDeriveTest` preserved). `tests/CMakeLists.txt` owns the `run_tests` target — root `CMakeLists.txt` only does `add_subdirectory(tests)`.
- No scratch files: root `test_*.cpp` probes and binaries were deleted. `.gitignore` covers `build/` and probe binaries.

## Registration macros

- Intrusive (any access level, befriends `debug_derive::DebugAccess`, preserves aggregates): `DEBUG_FIELDS(a, b)` inside class body.
- Zero args = auto-discovery for public aggregates (no dependency, real member names via `__PRETTY_FUNCTION__` folding, Section 4b): `DEBUG_FIELDS()` inside body, or `DEBUG_STRUCT(Type)` / `DEBUG_AUTO(Type)` at global scope. Any public aggregate also prints with no macro at all. Explicit listing always wins over auto.
- Auto limits: aggregates only (no private members, no user ctors, no virtuals/bitfields), up to 64 fields, no unions. Non-aggregate with zero args compiles but prints `Type {}` — list fields explicitly instead. `IsAutoReflectable<T>` concept gates the engine fallback.
- `DEBUG_STRUCT(Type, x, y)` is the explicit non-intrusive form. For private members via `DEBUG_STRUCT`, the class must add `DEBUG_ALLOW_EXTERNAL_FRIEND(Type)`.
- Enums are opt-in: `DEBUG_ENUM(Type, A, B)`. Registered → `Type::Variant`; unregistered → `TypeName(value)` fallback. Always add/update `tests/test_*.cpp` expectations when changing enum or formatting behavior.
- Pointers: null (raw/smart/C-string) → `nullptr`; non-null smart/raw → dereference and print (void*/function pointers keep the address).

## Hard limits (by design)

- Max 64 fields per macro (`CPPDERIVE_PP_MAP_0..64`); more fails to compile.
- `DebugContext`: `max_depth = 32` → prints `<recursion limit reached>`; cycle buffer is stack-allocated `visited_addresses[128]` with `<cycle: Type@addr>` output. Deep-chain and cycle tests (`CycleDetection`, `DepthLimiting` in `tests/test_safety.cpp`) pin this — keep them passing.
