cmake_minimum_required(VERSION 3.23)
project(Sudoku_23B)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Добавляем исходные файлы модели
add_library(model STATIC
    model/SudokuBoard.cpp
    model/SudokuSolver.cpp
)
target_include_directories(model PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/model
)

# Добавляем исходные файлы представления
add_library(view STATIC
    view/MainWindow.cpp
    view/SudokuCell.cpp
    model/SudokuBoard.cpp
    model/SudokuSolver.cpp
)
target_include_directories(view PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/view
    ${CMAKE_CURRENT_SOURCE_DIR}/model
)
target_link_libraries(view PRIVATE model Qt6::Widgets)

# Главное приложение
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE view model Qt6::Widgets)
