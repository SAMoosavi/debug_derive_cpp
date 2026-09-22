add_executable(run_tests
    test_primitives.cpp
    test_strings.cpp
    test_enums.cpp
    test_structs.cpp
    test_containers.cpp
    test_pointers.cpp
    test_auto.cpp
    test_private.cpp
    test_templates.cpp
    test_safety.cpp
)

target_link_libraries(run_tests PRIVATE debug_derive GTest::gtest_main)

if(MSVC)
  target_compile_options(run_tests PRIVATE /W4 /permissive-)
else()
  target_compile_options(run_tests PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(NAME AllTests COMMAND run_tests)

# Quarantined Debug-only auto-private TU: zero-arg DEBUG_FIELDS() printing
# private/protected via a TU-local `#define private public` hack (see the
# file header). Scoped to this target only — no global add_definitions, and
# run_tests sources/flags above are untouched. In Debug the hack is active
# (NDEBUG absent); in Release it compiles to the documented `Type {}` fallback.
add_executable(debug_auto_private test_debug_auto_private.cpp)

target_link_libraries(debug_auto_private PRIVATE debug_derive GTest::gtest_main)

if(MSVC)
  target_compile_options(debug_auto_private PRIVATE /W4 /permissive-)
else()
  target_compile_options(debug_auto_private PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(NAME AutoPrivateDebug COMMAND debug_auto_private)
