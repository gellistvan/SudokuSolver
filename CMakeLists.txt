cmake_minimum_required(VERSION 3.9)

project(SudokuSolver VERSION 1.0 LANGUAGES CXX)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 20) # Set C++17 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Ensure -std=c++17 is added to the compile options

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

add_definitions(-DCMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

option(BUILD_TESTING "Build unit and integ tests" OFF)

# Add coverage flags if building tests in Debug mode
#if(BUILD_TESTING AND CMAKE_BUILD_TYPE STREQUAL "Debug")
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 --coverage -fprofile-arcs -ftest-coverage")
#    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 --coverage -fprofile-arcs -ftest-coverage")
#    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
#endif()

#find_package(GTest)
#if(GTEST_FOUND)
#    message(STATUS "GTest was found!")
#    message(STATUS "GTest library: ${GTEST_LIBRARIES}")
#    message(STATUS "GTest main library: ${GTEST_MAIN_LIBRARIES}")
#    message(STATUS "GTest include directory: ${GTEST_INCLUDE_DIRS}")
#    enable_testing()
#else()
#    set(BUILD_TESTING off)
#endif()

add_subdirectory(modules)

add_executable(sudoku_solver main.cc)
target_link_libraries(sudoku_solver sudoku_core sudoku_common "${CMAKE_THREAD_LIBS_INIT}")
