# Библиотека модели
add_library(model_lib STATIC
    SudokuBoard.cpp
    SudokuSolver.cpp
    SudokuGenerator.cpp
)

target_include_directories(model_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(model_lib PRIVATE
    Qt6::Widgets
)
