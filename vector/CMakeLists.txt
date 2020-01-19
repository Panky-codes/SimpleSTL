# Name the project after the exercise
project(Vector CXX)
include_directories(${PROJECT_SOURCE_DIRECTORIES} .)
add_executable(Vector vector.h test/tests-main.cpp)
# Executable with sanitizers 
add_executable(Vectordbg vector.h test/tests-main.cpp)
target_link_libraries(Vectordbg asan )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_target_properties(Vector PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED OFF
    CXX_EXTENSIONS OFF
    COMPILE_FLAGS "-Wall -g -Wextra -Wpedantic -Werror"
)
# Sanitizer enabled
set_target_properties(Vectordbg PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED OFF
    CXX_EXTENSIONS OFF
    COMPILE_FLAGS "-Wall -Wextra -fsanitize=address -Wpedantic -Werror"
)
# Run the tests on every build  
add_custom_target(test_vector ALL DEPENDS Vector COMMAND Vector)
