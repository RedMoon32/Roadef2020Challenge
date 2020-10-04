cmake_minimum_required(VERSION 3.15)
project(Roadef2020Challenge)

set(CMAKE_CXX_STANDARD 14)

include_directories(checkers structs solvers tests libs)

set(SOURCE_FILES
        structs/parser.h
        libs/json.hpp
        structs/parser.cpp
        checkers/checker.cpp
        checkers/checker.h
        checkers/partial_checker.cpp
        checkers/partial_checker.h
        solvers/random_solver.cpp
        solvers/random_solver.h
        solvers/improved_random_solver.cpp
        solvers/improved_random_solver.h solvers/abstract_solver.h
        )

#add_executable(Roadef2020Challenge
#        main.cpp
#        ${SOURCE_FILES}
#        )

add_executable(CheckerBenchmark
        checkers/checker_benchmark.cpp
        ${SOURCE_FILES} )


add_executable(Tests
        tests/test_all.cpp
        ${SOURCE_FILES}
        )

SET(CMAKE_CXX_FLAGS "-O0")
SET(CMAKE_C_FLAGS "-O0")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -Wa,-mbig-obj -std=gnu++14")