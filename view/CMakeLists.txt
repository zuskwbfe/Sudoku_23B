# Библиотека представления
add_library(view_lib STATIC
    MainWindow.cpp
    DifficultyDialog.cpp
    SudokuCell.cpp
    RecordsWindow.cpp
    MainWindow.h
    DifficultyDialog.h
    SudokuCell.h
    RecordsWindow.h
)

target_include_directories(view_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/model  # для доступа к классам модели
    ${CMAKE_SOURCE_DIR}/controller
    )

target_link_libraries(view_lib PRIVATE
    Qt6::Widgets
    controller_lib
    model_lib  # зависимость от модели
)
