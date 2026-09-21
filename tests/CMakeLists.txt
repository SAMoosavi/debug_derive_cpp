add_executable(run_tests
    test_primitives.cpp
    test_strings.cpp
    test_enums.cpp
    test_structs.cpp
    test_containers.cpp
    test_pointers.cpp
    test_auto.cpp
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
