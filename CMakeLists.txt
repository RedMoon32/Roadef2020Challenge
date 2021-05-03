cmake_minimum_required(VERSION 3.10)
project(Roadef2020Challenge)

set(CMAKE_CXX_STANDARD 14)

include_directories(checkers structs solvers tests libs)

set(SOURCE_FILES
        libs/json.hpp

        checkers/abstract_checker.h
        checkers/checker.cpp
        checkers/checker.h

        solvers/random_solver.cpp
        solvers/random_solver.h
        solvers/hill_climbing.cpp
        solvers/hill_climbing.h
        solvers/abstract_solver.h
        solvers/abstract_solver.cpp
        solvers/genetic_solver.h
        solvers/genetic_solver.cpp

        structs/parser.cpp
        structs/structs.h
        structs/parser.h
        )


add_executable(challengeRTE
        checkers/checker_benchmark.cpp
        ${SOURCE_FILES})


find_package(Threads)
target_link_libraries(challengeRTE ${CMAKE_THREAD_LIBS_INIT})

SET(CMAKE_CXX_FLAGS "-O3")
SET(CMAKE_C_FLAGS "-O3")

#-Wa,-mbig-obj
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -std=gnu++14")