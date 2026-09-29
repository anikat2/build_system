cmake_minimum_required(VERSION 3.14)
project(build_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# google test compatibility
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# these come after googletest on purpose, so gtest itself is built
# without warning and sanitizer flags
if(MSVC)
  add_compile_options(/W4 /fsanitize=address)
  add_compile_definitions(_DISABLE_STRING_ANNOTATION _DISABLE_VECTOR_ANNOTATION)
else()
  add_compile_options(-Wall -Wextra -fsanitize=address -g)
  add_link_options(-fsanitize=address)
endif()

add_library(build_system_lib STATIC
  src/graph.cpp
  src/parser.cpp
  src/hasher.cpp
  src/executor.cpp
)
target_include_directories(build_system_lib PUBLIC include)

add_executable(build_system src/main.cpp)
target_link_libraries(build_system PRIVATE build_system_lib)

enable_testing()
include(GoogleTest)

add_executable(build_system_tests
  tests/graph_test.cpp
  tests/parser_test.cpp
  tests/hasher_test.cpp
  tests/executor_test.cpp
)
target_link_libraries(build_system_tests PRIVATE build_system_lib gtest_main)
gtest_discover_tests(build_system_tests)