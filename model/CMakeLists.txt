cmake_minimum_required(VERSION 3.23)

set(project "model")
project(${project})

set(CMAKE_CXX_STANDARD 17)

set(${project}_SOURCES
        SudokuBoard.cpp SudokuSolver.cpp)

set(${project}_HEADERS
SudokuBoard.h SudokuSolver.h)

set(${project}_SOURCE_LIST
        ${${project}_SOURCES}
        ${${project}_HEADERS})

add_library(${project}
        STATIC
        ${${project}_SOURCE_LIST})


# target_link_libraries(model PRIVATE Qt6::Widgets) ???
