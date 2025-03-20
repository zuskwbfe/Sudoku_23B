#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  QWidget *centralWidget = new QWidget(this);

  gridLayout = new QGridLayout(centralWidget);

  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      cells[i][j] = new SudokuCell(i, j);
      gridLayout->addWidget(cells[i][j], i, j);
    }
  }
  setCentralWidget(centralWidget);
}

void MainWindow::UpdateCell(int row, int col, int value) {
  cells[row][col]->setDisplayValue(value);
}
