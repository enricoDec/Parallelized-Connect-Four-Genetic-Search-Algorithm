cmake_minimum_required(VERSION 3.16)
project(src C)

set(CMAKE_C_STANDARD 11)

include_directories(includes)

# Find Raylib
find_package(raylib 3.7 REQUIRED)

add_executable(src
        includes/argparse.h
        includes/benchmark.h
        includes/game.h
        includes/geneticSearch.h
        argparse.c
        benchmark.c
        game.c
        geneticSearch.c
        main.c)

