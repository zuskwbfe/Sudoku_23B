# Библиотека представления
add_library(view_lib STATIC
    MainWindow.cpp
    SudokuCell.cpp
)

target_include_directories(view_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/model  # для доступа к классам модели
)

target_link_libraries(view_lib PRIVATE
    Qt6::Widgets
    model_lib  # зависимость от модели
)
