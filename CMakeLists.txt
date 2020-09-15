cmake_minimum_required(VERSION 3.15)
project(Roadef2020Challenge)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        parser.h
        json.h
        parser.cpp
        checker.cpp
        checker.h
        solver.cpp
        solver.h
        improved_solver.cpp
        improved_solver.h)

#add_executable(Roadef2020Challenge
#        main.cpp
#        ${SOURCE_FILES}
#        )

add_executable(CheckerBenchmark
        checker_benchmark.cpp
        ${SOURCE_FILES})


add_executable(Tests
        tests/test_all.cpp
        ${SOURCE_FILES}
        )

SET(CMAKE_CXX_FLAGS "-O0")
SET(CMAKE_C_FLAGS "-O0")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -Wa,-mbig-obj -std=gnu++14")