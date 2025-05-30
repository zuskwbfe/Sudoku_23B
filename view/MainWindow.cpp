#include "MainWindow.h"
#include "../model/SudokuBoard.h"
#include "../controller/SudokuController.h"
#include <QInputDialog>
#include <QLabel>
#include <QApplication>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  // Инициализация массива ячеек нулевыми указателями
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      cells[i][j] = nullptr;
    }
  }

  stackedWidget = new QStackedWidget(this);
  setCentralWidget(stackedWidget);

  createMainMenu();
  createGameScreen();

  difficultyDialog = new DifficultyDialog(this);
  stackedWidget->addWidget(menuScreen);
  stackedWidget->addWidget(gameScreen);
}

// Обновление отображаемого значения в ячейке
void MainWindow::UpdateCell(int row, int col, int value) {
  if (row < 0 || row >= 9 || col < 0 || col >= 9) return;

  // Проверка существования ячейки
  if (!cells[row][col]) {
      qWarning() << "Cell not initialized at" << row << col;
      return;
  }

  // Обновляем значение ячейки
  cells[row][col]->setDisplayValue(value);

  // Устанавливаем флаг "оригинальности" только если контроллер доступен
  if (controller && controller->getBoard()) {
    cells[row][col]->setOriginal(controller->getBoard()->isCellOriginal(row, col));
  }
}

SudokuCell *MainWindow::getCell(int row, int col) {
  if (row < 0 || row >= 9 || col < 0 || col >= 9)
    return nullptr;

  // Проверка инициализации ячейки
  if (!cells[row][col]) {
      qWarning() << "Requested null cell at" << row << col;
      return nullptr;
  }

  return cells[row][col];
}
void MainWindow::updateBoard() {
  if (!controller) return;
  SudokuBoard* board = controller->getBoard();
  if (!board) return;

  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      cells[i][j]->setDisplayValue(board->getCellValue(i, j));
      cells[i][j]->setOriginal(board->isCellOriginal(i, j));
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

void MainWindow::createMainMenu() {
  menuScreen = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout(menuScreen);

  QPushButton *newGameBtn = new QPushButton("Новая игра");
  QPushButton *exitBtn = new QPushButton("Выход");

  layout->addStretch();
  layout->addWidget(newGameBtn);
  layout->addWidget(exitBtn);
  layout->addStretch();

  connect(newGameBtn, &QPushButton::clicked, this, &MainWindow::handleNewGame);
  connect(exitBtn, &QPushButton::clicked, qApp, &QApplication::quit);
}

void MainWindow::createGameScreen() {
  gameScreen = new QWidget;
  QVBoxLayout *mainLayout = new QVBoxLayout(gameScreen);

  // Панель информации
  QHBoxLayout *infoLayout = new QHBoxLayout();
  timerLabel = new QLabel("Время: 00:00");
  errorLabel = new QLabel("Ошибки: 0");

  // Кнопка возврата
  QPushButton *backBtn = new QPushButton("В меню");
  connect(backBtn, &QPushButton::clicked, this, &MainWindow::showMainMenu);

  // Игровая сетка
  gridLayout = new QGridLayout();

  // Добавление элементов
  infoLayout->addWidget(timerLabel);
  infoLayout->addWidget(errorLabel);
  mainLayout->addLayout(infoLayout);
  mainLayout->addWidget(backBtn);
  mainLayout->addLayout(gridLayout);

  // Инициализация ячеек с оригинальной стилизацией
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      cells[row][col] = new SudokuCell(row, col);
      gridLayout->addWidget(cells[row][col], row, col);
      connect(cells[row][col], &SudokuCell::cellClicked,
              this, &MainWindow::handleCellSelected);

      // Подключение сигналов
      connect(cells[row][col], &SudokuCell::cellDoubleClicked,
              this, &MainWindow::handleCellDoubleClicked);
    }
  }
}

void MainWindow::handleNewGame() {
  if (difficultyDialog->exec() == QDialog::Accepted) {
    stackedWidget->setCurrentIndex(1);
    emit controller->newGame(difficultyDialog->selectedDifficulty());
  }
}

void MainWindow::showMainMenu() {
  stackedWidget->setCurrentIndex(0);
  controller->stopTimer();
}

void MainWindow::handleGameFinished() {
  showMainMenu();
  QMessageBox::information(this, "Поздравляем!", "Игра завершена!");
}

// Для обработки выбора ячейки
void MainWindow::handleCellSelected(int row, int col) {
  if (!controller || !controller->isGameStarted()) return;

  SudokuBoard* board = controller->getBoard();
  if (!board) return;

  clearHighlights();
  highlightRelatedCells(row, col);

  // Подсвечиваем одинаковые цифры
 int clickedValue = board->getCellValue(row, col);

  // Если ячейка не пустая
  if (clickedValue > 0) {
    for (int r = 0; r < 9; ++r) {
      for (int c = 0; c < 9; ++c) {
        // Пропускаем текущую ячейку (она уже подсвечена)
        if (r == row && c == col) continue;

        // Если значение совпадает - подсвечиваем
        if (board->getCellValue(r, c) == clickedValue) {
          cells[r][c]->setHighlightState(SudokuCell::SameDigit);
        }
      }
    }
  }

  // Испускаем сигнал для контроллера
  emit CellClicked(row, col);
}

// Сбрасываем подсветку во всех ячейках
void MainWindow::clearHighlights() {
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      cells[row][col]->setHighlightState(SudokuCell::Default);
    }
  }
}

// Подсвечиваем связанные ячейки
void MainWindow::highlightRelatedCells(int row, int col) {
  // Подсветка строки и столбца
  for (int i = 0; i < 9; ++i) {
    cells[row][i]->setHighlightState(SudokuCell::Related);
    cells[i][col]->setHighlightState(SudokuCell::Related);
  }

  // Подсветка блока 3x3
  int blockRow = (row / 3) * 3;
  int blockCol = (col / 3) * 3;
  for (int r = blockRow; r < blockRow + 3; ++r) {
    for (int c = blockCol; c < blockCol + 3; ++c) {
      cells[r][c]->setHighlightState(SudokuCell::Related);
    }
  }

  // Подсветка самой ячейки
  cells[row][col]->setHighlightState(SudokuCell::Selected);
}

// Подсвечиваем ячейки с той же цифрой
void MainWindow::highlightSameDigits(int row, int col) {
  if (!controller) return;

  SudokuBoard* board = controller->getBoard(); // Получаем актуальную доску
  if (!board) return;

  int clickedValue = board->getCellValue(row, col);
  // Если ячейка не пустая
  if (clickedValue > 0) {
    for (int r = 0; r < 9; ++r) {
      for (int c = 0; c < 9; ++c) {
        // Пропускаем текущую ячейку (она уже подсвечена)
        if (r == row && c == col) continue;

        // Если значение совпадает - подсвечиваем
        if (board->getCellValue(r, c) == clickedValue) {
          cells[r][c]->setHighlightState(SudokuCell::SameDigit);
        }
      }
    }
  }
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (controller && controller->isGameStarted()) {
        int row = controller->selectedRow();
        int col = controller->selectedCol();

        if (row >= 0 && col >= 0) {
            int key = event->key();

            // Обработка цифр 1-9
            if (key >= Qt::Key_1 && key <= Qt::Key_9) {
                int value = key - Qt::Key_0;
                emit numberEntered(row, col, value);
            }
            // Обработка Backspace и Delete для очистки
            else if (key == Qt::Key_Backspace || key == Qt::Key_Delete) {
                emit numberEntered(row, col, 0);
            }
        }
    }

    QMainWindow::keyPressEvent(event);
}

// Слот для обработки двойного клика
void MainWindow::handleCellDoubleClicked(int row, int col) {
  // Испускаем специальный сигнал для двойного клика
  emit CellDoubleClicked(row, col);
}

void MainWindow::setController(SudokuController *ctrl) {
    controller = ctrl;
    // Принудительно обновляем доску при установке контроллера
    if (controller && controller->getBoard()) {
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                UpdateCell(row, col, controller->getBoard()->getCellValue(row, col));
            }
        }
    }
}

void MainWindow::refreshHighlight() {
    if (controller && controller->isGameStarted()) {
        int row = controller->selectedRow();
        int col = controller->selectedCol();

        if (row >= 0 && col >= 0) {
            handleCellSelected(row, col);
        }
    }
}
