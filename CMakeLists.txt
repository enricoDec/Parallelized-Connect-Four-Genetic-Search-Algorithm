cmake_minimum_required(VERSION 3.12)
project(GameProject C)
set(CMAKE_C_STANDARD 99)

# Set the source directories
set(SRCDIR src)
set(TESTDIR test)
set(BUILDDIR build)
set(BINDIR bin)

# Set the sources and objects
file(GLOB SOURCES ${SRCDIR}/*.c)
file(GLOB TESTSOURCES ${TESTDIR}/*.c)
list(FILTER SOURCES EXCLUDE REGEX "${SRCDIR}/main.c")
list(FILTER SOURCES EXCLUDE REGEX "${SRCDIR}/gameUI.c")
list(FILTER SOURCES EXCLUDE REGEX "${SRCDIR}/test.c")
set(OBJECTS "")
foreach(SRC ${SOURCES})
    get_filename_component(OBJ ${SRC} NAME_WE)
    set(OBJ ${BUILDDIR}/${OBJ}.o)
    add_custom_command(
        OUTPUT ${OBJ}
        COMMAND ${CMAKE_C_COMPILER} ${CFLAGS} -c ${SRC} -o ${OBJ}
        DEPENDS ${SRC}
    )
    list(APPEND OBJECTS ${OBJ})
endforeach()

# Set the target executables
add_executable(app ${SRCDIR}/main.c ${SRCDIR}/gameUI.c ${OBJECTS})
add_executable(test ${TESTSOURCES} ${OBJECTS})
set_target_properties(app PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BINDIR})
set_target_properties(test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BINDIR})

# Set compiler flags and libraries
find_package(raylib REQUIRED)
target_include_directories(app PRIVATE ${raylib_INCLUDE_DIRS})
target_link_libraries(app PRIVATE ${raylib_LIBRARIES} m)
target_compile_options(app PRIVATE ${CFLAGS} ${OpenMP_C_FLAGS})
target_include_directories(test PRIVATE ${raylib_INCLUDE_DIRS})
target_link_libraries(test PRIVATE ${raylib_LIBRARIES} m)
target_compile_options(test PRIVATE ${CFLAGS} ${OpenMP_C_FLAGS} -DTEST)

# Additional build targets
add_custom_target(build DEPENDS app)
add_custom_target(release DEPENDS build)
add_custom_target(tests DEPENDS test)

# Enable OpenMP
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    target_link_libraries(app PRIVATE OpenMP::OpenMP_C)
    target_link_libraries(test PRIVATE OpenMP::OpenMP_C)
endif()
