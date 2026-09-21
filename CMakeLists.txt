cmake_minimum_required(VERSION 3.20)

project(CppDebugDerive VERSION 0.1.0 LANGUAGES CXX)

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(BUILD_DEMO "Build the demo executable" ON)
option(BUILD_TESTING "Build the test suite" ON)

# ---------------------------------------------------------------------------
# Header-only library target
# ---------------------------------------------------------------------------
add_library(debug_derive INTERFACE)
target_include_directories(debug_derive INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_features(debug_derive INTERFACE cxx_std_20)

# Keep demo/test binaries at the top-level binary dir so the documented
# `./build/demo` and `./build/run_tests` paths stay valid even when
# `run_tests` is defined inside the tests/ subdirectory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# ---------------------------------------------------------------------------
# Demo
# ---------------------------------------------------------------------------
if(BUILD_DEMO)
  add_executable(demo examples/demo.cpp)
  target_link_libraries(demo PRIVATE debug_derive)
  if(MSVC)
    target_compile_options(demo PRIVATE /W4 /permissive-)
  else()
    target_compile_options(demo PRIVATE -Wall -Wextra -pedantic)
  endif()
endif()

# ---------------------------------------------------------------------------
# Testing: GTest provider for tests/ (which owns the `run_tests` target)
# ---------------------------------------------------------------------------
include(CTest)

if(BUILD_TESTING)
  if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
  endif()

  find_package(GTest QUIET)

  if(NOT GTest_FOUND)
    message(STATUS "System GTest not found, fetching googletest v1.14.0 via FetchContent")
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
      URL_HASH SHA256=1f357c27ca988c3f7c6b4bf68a9395005ac6761f034046e9dde0896e3aba00e4
    )
    # Prevent overriding parent project compiler/linker settings on Windows.
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
  endif()

  # Contract for tests/CMakeLists.txt (maintained separately): it must define
  #   add_executable(run_tests <sources>)
  #   target_link_libraries(run_tests PRIVATE debug_derive GTest::gtest_main)
  #   target_compile_options(run_tests PRIVATE -Wall -Wextra -pedantic)  # non-MSVC
  #   add_test(NAME AllTests COMMAND run_tests)
  # `GTest::gtest_main` is provided both by find_package(GTest) and by the
  # FetchContent fallback above (googletest v1.14.0 exposes GTest:: aliases).
  add_subdirectory(tests)
endif()

# ---------------------------------------------------------------------------
# Install rules (header + CMake package config)
# ---------------------------------------------------------------------------
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(
  FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/debug_derive.hpp"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
install(
  TARGETS debug_derive
  EXPORT CppDebugDeriveTargets
)
install(
  EXPORT CppDebugDeriveTargets
  FILE CppDebugDeriveTargets.cmake
  NAMESPACE CppDebugDerive::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CppDebugDerive"
)

# Generate the package config files in the binary dir so no extra source
# template file is needed.
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/CppDebugDeriveConfig.cmake.in"
  "@PACKAGE_INIT@\n"
  "include(\"${CMAKE_CURRENT_LIST_DIR}/CppDebugDeriveTargets.cmake\")\n"
  "check_required_components(CppDebugDerive)\n"
)
configure_package_config_file(
  "${CMAKE_CURRENT_BINARY_DIR}/CppDebugDeriveConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/CppDebugDeriveConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CppDebugDerive"
)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/CppDebugDeriveConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMajorVersion
)
install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/CppDebugDeriveConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/CppDebugDeriveConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CppDebugDerive"
)
