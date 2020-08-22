cmake_minimum_required(VERSION 3.15)
project(Roadef2020Challenge)

set(CMAKE_CXX_STANDARD 14)

add_executable(Roadef2020Challenge
        solution/data_reader.cpp
        solution/solution_template.cpp
        main.cpp
        cchecker.h
        json.h
        cchecker.cpp
        )
