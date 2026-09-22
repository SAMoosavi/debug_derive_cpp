# automatic-nested
## 1 Goal — Nested/user types auto-recurse in DEBUG so no silent `<<`/`<unformattable>` fallback.
## 2 Scope in — IsDebugReflectable detect; explicit-if-present; generic auto recurse; notice+marker; const/ref/value preserved. Out — NOT alter explicit format/unions/bitfields/virtuals/RTTI/heap.
## 3 Stories — When nested has DEBUG system SHALL use explicit; When aggregate lacks DEBUG system SHALL generic-auto recurse; When unsupported system SHALL emit notice + runtime marker.
## 4 Acceptance — Given with-DEBUG nested / When printed / Then output == explicit format byte-identical.
## 4 Acceptance — Given without-DEBUG aggregate / When printed / Then generic auto fields + marker + compiler notice where feasible.
## 4 Acceptance — Given 3-level nesting + mixed primitive/container/with/without / When printed / Then all levels correct, depth≤32, cycles→`<cycle>`.
## 5 Edge+constraints — Distinguish primitive/container/with-DEBUG/without-DEBUG/unsupported; no per-type duplication; no RTTI/heap; reuse depth 32/cycle 128; const/ref/value identical.
## 6 Open questions — None (ponytail defaults: runtime marker + pragma/deprecated message where feasible, formatting unchanged).
## 7 Files+Tasks — `include/debug_derive.hpp`, `tests/test_types.hpp`, `tests/test_*.cpp`, `README.md`, `examples/demo.cpp`.
## 7 Tasks — 1. Failing tests: primitive/with/without/multi-level/mixed [P] 2. Min recurse dispatch (reflectable→explicit else auto else marker) 3. Notice+marker+docs/demo update.
