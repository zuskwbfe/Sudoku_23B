#include "model/SudokuBoard.h"
#include "model/SudokuSolver.h"
#include "view/MainWindow.h"
#include <QApplication> // Базовый класс приложения Qt
#include <QMessageBox>  // Для показа сообщений
#include <QPushButton>  // Класс кнопки

int main(int argc, char *argv[]) {
  // Создаем экземпляр приложения Qt
  QApplication app(argc, argv);

  // Создаем главное окно приложения
  MainWindow window;

  // Виджет для панели управления
  QWidget controlPanel;

  // Вертикальный layout для расположения кнопок
  QVBoxLayout layout(&controlPanel);

  // Создаем кнопки управления
  QPushButton solveButton("Решить судоку");
  QPushButton clearButton("Очистить доску");
  QPushButton exampleButton("Пример судоку");

  // Добавляем кнопки в layout
  layout.addWidget(&solveButton);
  layout.addWidget(&clearButton);
  layout.addWidget(&exampleButton);

  // Устанавливаем layout для панели управления
  controlPanel.setLayout(&layout);

  // Показываем панель управления
  controlPanel.show();

  // Обработчик кнопки "Решить судоку"
  QObject::connect(&solveButton, &QPushButton::clicked, [&window]() {
    // Создаем решатель
    BacktrackingSolver solver;

    // Пытаемся решить судоку
    if (solver.solve(window.getBoard())) {
      // Если решение найдено - обновляем отображение
      window.updateBoard();
    } else {
      // Если решение не найдено - показываем сообщение об ошибке
      QMessageBox::warning(&window, "Ошибка", "Не удалось решить судоку!");
    }
  });

  // Обработчик кнопки "Очистить доску"
  QObject::connect(&clearButton, &QPushButton::clicked, [&window]() {
    // Очищаем доску
    window.getBoard().clear();
    // Обновляем отображение
    window.updateBoard();
  });

  // Обработчик кнопки "Пример судоку"
  QObject::connect(&exampleButton, &QPushButton::clicked, [&window]() {
    // Получаем ссылку на доску
    SudokuBoard &board = window.getBoard();

    // Очищаем доску перед заполнением примера
    board.clear();

    // Пример начальной расстановки (известный решаемый судоку)
    int example[9][9] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0}, {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0}, {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1}, {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0}, {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}};

    // Заполняем доску значениями из примера
    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        board.setCellValue(i, j, example[i][j]);
      }
    }

    window.updateBoard();
  });

  window.show();

  // Запускаем главный цикл обработки событий приложения
  return app.exec();
}
