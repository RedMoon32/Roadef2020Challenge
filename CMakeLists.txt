cmake_minimum_required(VERSION 3.15)
project(Roadef2020Challenge)

set(CMAKE_CXX_STANDARD 14)

add_executable(Roadef2020Challenge
        solution/data_reader.cpp
        solution/solution_template.cpp
        main.cpp
        parser.h
        json.h
        parser.cpp
        checker.cpp
        checker.h
        )

add_executable(Tests
        tests/test_all.cpp
        parser.h
        json.h
        parser.cpp
        checker.h
        checker.cpp
        )

SET(CMAKE_CXX_FLAGS "-O0")
SET(CMAKE_C_FLAGS "-O0")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -Wa,-mbig-obj -std=gnu++14" )