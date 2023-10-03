cmake_minimum_required(VERSION 3.16)
project(src C)

include_directories(includes)

# Find raylib package
find_package(raylib 3.0 REQUIRED)

# Configure C compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS} -O3")  # Add optimization flags

# Linker flags (LDFLAGS)
set(LDFLAGS "${LDFLAGS} -lm -fopenmp")  # Add linker flags

add_executable(src
        includes/argparse.h
        includes/benchmark.h
        includes/game.h
        includes/gameUI.h
        includes/geneticSearch.h
        includes/raygui.h
        argparse.c
        benchmark.c
        game.c
        gameUI.c
        geneticSearch.c
        main.c)

# Link raylib library to your project
target_link_libraries(src PRIVATE raylib)

find_package(OpenMP REQUIRED)
if(OpenMP_C_FOUND)
    target_link_libraries(src PRIVATE OpenMP::OpenMP_C)
endif()