cmake_minimum_required(VERSION 3.5)
project(Sudoku)

# настройка Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories(
    ${CMAKE_SOURCE_DIR}/model
    ${CMAKE_SOURCE_DIR}/view
    ${CMAKE_SOURCE_DIR}/controller
)

add_subdirectory(model)
add_subdirectory(view)
add_subdirectory(controller)

add_executable(Sudoku main.cpp)
target_link_libraries(Sudoku
    PRIVATE
    model_lib
    view_lib
    controller_lib
    Qt6::Widgets
)
