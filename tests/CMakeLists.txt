# Name the project after the exercise
project(Vector CXX)

add_executable(Vector tests-vector.cpp)
add_test(test_vector Vector)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
target_include_directories(Vector PUBLIC ${PROJECT_SOURCE_DIR}/../vector)
set_target_properties(Vector PROPERTIES
    CXX_STANDARD 14
    CXX_EXTENSIONS OFF
    COMPILE_FLAGS "-Wall -g -Wextra -Wpedantic -Werror"
)
