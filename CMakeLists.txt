cmake_minimum_required(VERSION 3.10)
project(Roadef2020Challenge)

set(CMAKE_CXX_STANDARD 14)

include_directories(source/checkers source/structs source/solvers source/tests source/libs)

set(SOURCE_FILES
        source/libs/json.hpp

        source/checkers/abstract_checker.h
        source/checkers/checker.cpp
        source/checkers/checker.h

        source/solvers/random_solver.cpp
        source/solvers/random_solver.h
        source/solvers/hill_climbing.cpp
        source/solvers/hill_climbing.h
        source/solvers/abstract_solver.h
        source/solvers/abstract_solver.cpp
        source/solvers/genetic_solver.h
        source/solvers/genetic_solver.cpp

        source/structs/parser.cpp
        source/structs/structs.h
        source/structs/parser.h
        )


add_executable(challengeRTE
        source/checkers/checker_benchmark.cpp
        ${SOURCE_FILES})


find_package(Threads)
target_link_libraries(challengeRTE ${CMAKE_THREAD_LIBS_INIT})

SET(CMAKE_CXX_FLAGS "-O3")
SET(CMAKE_C_FLAGS "-O3")

#-Wa,-mbig-obj
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -std=gnu++14")