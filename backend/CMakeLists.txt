cmake_minimum_required(VERSION 3.14)
project(OS_Scheduling_Simulator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure directories
include_directories(include)

# Find all source files for the core and algorithms
file(GLOB_RECURSE SOURCES "src/core/*.cpp" "src/algorithms/*.cpp" "src/utils/*.cpp")

# Create the main executable (currently a CLI for testing, UI will be added later)
add_executable(OSScheduler src/main.cpp ${SOURCES})

# Create the JSON CLI Wrapper for the Frontend
add_executable(OSServer src/server.cpp ${SOURCES})

# --- GoogleTest Setup ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Helper macro to add tests easily
macro(add_os_test TEST_NAME FILE_NAME)
    add_executable(${TEST_NAME} tests/${FILE_NAME} ${SOURCES})
    target_link_libraries(${TEST_NAME} GTest::gtest_main)
    target_include_directories(${TEST_NAME} PRIVATE include)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endmacro()

add_os_test(test_fcfs test_fcfs.cpp)
add_os_test(test_sjf test_sjf.cpp)
add_os_test(test_priority test_priority.cpp)
add_os_test(test_hrrn test_hrrn.cpp)
add_os_test(test_rr test_rr.cpp)
add_os_test(test_srtf test_srtf.cpp)
add_os_test(test_preemptive_priority test_preemptive_priority.cpp)
add_os_test(test_mlq test_mlq.cpp)
add_os_test(test_csv test_csv.cpp)
