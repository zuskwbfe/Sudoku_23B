cmake_minimum_required(VERSION 3.23)

set(project "view")
project(${project})

set(CMAKE_CXX_STANDARD 17)

set(${project}_SOURCES
        MainWindow.cpp SudokuCell.cpp)

set(${project}_HEADERS
        MainWindow.h SudokuCell.h)

set(${project}_SOURCE_LIST
        ${${project}_SOURCES}
        ${${project}_HEADERS})

add_library(${project}
        STATIC
        ${${project}_SOURCE_LIST})


target_link_libraries(view PRIVATE Qt6::Widgets)
