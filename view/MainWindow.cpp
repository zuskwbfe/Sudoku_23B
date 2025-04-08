#include "MainWindow.h"
#include "../model/SudokuBoard.h"
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  // Создаем центральный виджет для размещения элементов
  QWidget *centralWidget = new QWidget(this);

  // Инициализируем сетку 9x9 для расположения ячеек
  gridLayout = new QGridLayout(centralWidget);

  // Убрираем промежутки между ячейками
  gridLayout->setSpacing(0);

  // Заполняем сетку ячейками SudokuCell
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      cells[i][j] = new SudokuCell(i, j);
      gridLayout->addWidget(cells[i][j], i, j);

      // Настройка стилей границ для разделения блоков
      QString style = "SudokuCell {"
                      "  border: none;"
                      "  margin: 0;"
                      "  padding: 0;"
                      "  border-right: 1px solid black;"
                      "  border-bottom: 1px solid black;"
                      "}";

      // Утолщение границ между блоками (столбцы 2, 5)
      if (j == 2 || j == 5)
        style += "SudokuCell { border-right-width: 2px; }";

      // Утолщение границ между блоками (строки 2, 5)
      if (i == 2 || i == 5)
        style += "SudokuCell { border-bottom-width: 2px; }";

      // Убрать границы у последнего столбца и строки
      if (j == 8)
        style += "SudokuCell { border-right: none; }";
      if (i == 8)
        style += "SudokuCell { border-bottom: none; }";

      cells[i][j]->setStyleSheet(style);

      // Подключаем сигнал клика ячейки к слоту CellClicked
      connect(cells[i][j], &SudokuCell::cellClicked, this,
              &MainWindow::CellClicked);
    }
  }
  // Устанавливаем центральный виджет и заголовок окна
  setCentralWidget(centralWidget);
  setWindowTitle("Sudoku");
}

// Обновление отображаемого значения в ячейке
void MainWindow::UpdateCell(int row, int col, int value) {
  cells[row][col]->setDisplayValue(value);
}

void MainWindow::setBoard(const SudokuBoard &newBoard) { board = newBoard; }

void MainWindow::updateBoard() {
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      cells[i][j]->setDisplayValue(board.getCellValue(i, j));
    }
  }
}
