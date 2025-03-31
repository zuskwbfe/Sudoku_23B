# Библиотека контроллера
add_library(controller_lib STATIC
    SudokuController.cpp
)

target_link_libraries(controller_lib PRIVATE
    Qt6::Widgets
    model_lib
    view_lib
)
