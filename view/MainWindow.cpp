#include "MainWindow.h"
#include "../model/SudokuBoard.h"
#include "../controller/SudokuController.h"
#include <QInputDialog>
#include <QLabel>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
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

      // Начальный стиль для всех ячеек
      QString style = "SudokuCell {"
                      "  border: none;"
                      "  margin: 0;"
                      "  padding: 0;"
                      "  border-right: 1px solid black;"
                      "  border-bottom: 1px solid black;"
                      "}";

      // Утолщение правой границы для столбцов 2 и 5
      if (col == 2 || col == 5) {
        style += "SudokuCell { border-right-width: 2px; }";
      }

      // Утолщение нижней границы для строк 2 и 5
      if (row == 2 || row == 5) {
        style += "SudokuCell { border-bottom-width: 2px; }";
      }

      // Удаление границ для последнего столбца и строки
      if (col == 8) {
        style += "SudokuCell { border-right: none; }";
      }
      if (row == 8) {
        style += "SudokuCell { border-bottom: none; }";
      }

      cells[row][col]->setStyleSheet(style);

      // Подключение сигналов
      connect(cells[row][col], &SudokuCell::cellClicked,
              [this, row, col]() { emit CellClicked(row, col); });
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
