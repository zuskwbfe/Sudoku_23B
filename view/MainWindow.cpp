#include "MainWindow.h"
#include "../model/SudokuBoard.h"
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  // Создаем центральный виджет для размещения элементов
  QWidget *centralWidget = new QWidget(this);

  // Инициализируем сетку 9x9 для расположения ячеек
  gridLayout = new QGridLayout(centralWidget);

  // Заполняем сетку ячейками SudokuCell
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      cells[i][j] = new SudokuCell(i, j);
      gridLayout->addWidget(cells[i][j], i, j);
      // Подключаем сигнал клика ячейки к слоту CellClicked
      connect(cells[i][j], &SudokuCell::cellClicked, this,
              &MainWindow::CellClicked); //
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

void MainWindow::setBoard(const SudokuBoard& newBoard) {
  board = newBoard;
}

void MainWindow::updateBoard() {
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      cells[i][j]->setDisplayValue(board.getCellValue(i, j));
    }
  }
}
