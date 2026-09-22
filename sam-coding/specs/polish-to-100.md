# polish-to-100
## 1 Goal — Reach 100/100 on merged develop: zero duplication drift, exact tests, CI-guarded NDEBUG/sanitizers.
## 2 Scope in — helper-only dedup, exact NestedPrivate, edge tests, -DNDEBUG CI job, ASan/UBSan+C++23 checks. Out — SHALL NOT change output bytes, public API, or 64-field/32-depth/128-cycle limits.
## 3 Stories — When struct formats the system SHALL use one shared helper; When NestedPrivate prints it SHALL equal one exact string; When Release/-DNDEBUG builds it SHALL keep fallback identical; When ASan/UBSan/C++23 runs it SHALL stay clean.
## 4 Acceptance — Given any struct/When printed/Then bytes identical pre/post helper. Given NestedPrivate/When printed/Then EXPECT_EQ exact, 0 EXPECT_NE remains. Given -DNDEBUG Release/When debug_auto_private runs/Then fallback `Type {}` passes in CI. Given ASan+UBSan+C++23/When suite runs/Then 0 failures.
## 5 Edge+constraints — rvalue private holder prints; private×cycle/depth combo hits guards; runtime unformattable-nested emits marker; NDEBUG overhead noted <1% (no new harness); helper touches only dispatch ~1479/1504 blocks.
## 6 Open questions — None.
## 7 Files+Tasks — include/debug_derive.hpp, tests/test_private.cpp:18, tests/CMakeLists.txt. 1 Failing exact NestedPrivate+edge tests (rvalue,cycle/depth,unformattable) [P] 2 Shared struct-format helper, bytes identical 3 Strip redundant EXPECT_NE 4 Release -DNDEBUG CI job+promote PrivateNDEBUGIdentical [P] 5 ASan/UBSan+C++23 check+bench note [P].
