#include "controller/SudokuController.h"
#include "model/SudokuBoard.h"
#include "model/SudokuGenerator.h"
#include "model/SudokuSolver.h"
#include "view/MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // инициализация частей
  SudokuBoard board;
  BacktrackingSolver solver;
  SudokuGenerator generator(&solver);
  MainWindow window;

  // генерация игрового поля
  generator.generate(board, 1); // средняя сложность (50 пустых клеток)

  // проверка что генерация работает правильно
  int emptyCells = 0;
  for (int r = 0; r < 9; r++) {
    for (int c = 0; c < 9; c++) {
      if (board.getCellValue(r, c) == 0)
        emptyCells++;
    }
  }
  qDebug() << "Empty cells generated:" << emptyCells;

  // настройка контроллера
  SudokuController controller(&board, &window);

  // настройка окна
  window.setWindowTitle("Судоку");
  window.resize(450, 450);
  window.show();

  return app.exec();
}
