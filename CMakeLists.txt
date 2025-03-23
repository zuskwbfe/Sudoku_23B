cmake_minimum_required(VERSION 3.23)

set(project "Sudoku_23B")
project(${project})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(${project}_SOURCES
        main.cpp)

add_subdirectory(view)
add_subdirectory(model)

set(${project}_SOURCE_LIST
        ${${project}_SOURCES})


add_executable(${project}
        ${${project}_SOURCE_LIST})

        target_link_libraries(Sudoku_23B
        PRIVATE
            Qt6::Widgets
            view
            model
    )
