#include "MainWindow.h"
#include "../model/SudokuBoard.h"
#include <QInputDialog>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  // Создаем центральный виджет для размещения элементов
  QWidget *centralWidget = new QWidget(this);
  QVBoxLayout *mainLayout =
      new QVBoxLayout(centralWidget); // Основной вертикальный layout

  // Создаём панель для таймера и ошибок
  QHBoxLayout *infoLayout = new QHBoxLayout();
  timerLabel = new QLabel("Время: 00:00", this);
  errorLabel = new QLabel("Ошибки: 0", this);

  infoLayout->addWidget(timerLabel);
  infoLayout->addWidget(errorLabel);
  mainLayout->addLayout(infoLayout);

  // Инициализируем сетку 9x9 для расположения ячеек
  gridLayout = new QGridLayout();
  mainLayout->addLayout(gridLayout);

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

SudokuCell *MainWindow::getCell(int row, int col) {
  if (row < 0 || row >= 9 || col < 0 || col >= 9)
    return nullptr;
  return cells[row][col];
}

void MainWindow::setBoard(const SudokuBoard &newBoard) {
  board = newBoard;
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      cells[row][col]->setOriginal(board.isCellOriginal(row, col));
      cells[row][col]->setDisplayValue(board.getCellValue(row, col));
    }
  }
}

void MainWindow::updateBoard() {
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      cells[i][j]->setDisplayValue(board.getCellValue(i, j));
    }
  }
}

void MainWindow::updateTimerDisplay(int seconds) {
  int minutes = seconds / 60;
  int secs = seconds % 60;
  timerLabel->setText(QString("Время: %1:%2")
                          .arg(minutes, 2, 10, QLatin1Char('0'))
                          .arg(secs, 2, 10, QLatin1Char('0')));
}

void MainWindow::updateErrorDisplay(int errors) {
  errorLabel->setText(QString("Ошибки: %1").arg(errors));
  errorLabel->setStyleSheet("color: red;");
}

void MainWindow::updateGameStats(const QString &time, int errors) {
  timerLabel->setText("Время: " + time);
  errorLabel->setText(QString("Ошибки: %1").arg(errors));
}
