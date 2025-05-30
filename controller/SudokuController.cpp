#include "SudokuController.h"
#include "SudokuGenerator.h"
#include <QInputDialog> // диалог ввода чисел
#include <QMessageBox>  // всплывающие сообщения

SudokuController::SudokuController(SudokuBoard *board, MainWindow *view,
                                   QObject *parent)
    : QObject(parent), board_(board), view_(view), solver_(),
      generator_(&solver_), selectedRow_(-1), selectedCol_(-1),
      gameStarted_(false) {
  // соединение сигнала клика по ячейке со слотом обработки
  connect(view_, &MainWindow::CellClicked, this, &SudokuController::handleCellInteraction);
  connect(view_, &MainWindow::CellDoubleClicked, this, &SudokuController::processCellInput);

  // Инициализация таймера
  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &SudokuController::updateTimer);
}

// Форматирует время в формат "мм:сс" для отображения на UI.
QString SudokuController::formatTime(int seconds) const {
  int minutes = seconds / 60;
  int secs = seconds % 60;
  return QString("%1:%2")
      .arg(minutes, 2, 10, QLatin1Char('0'))
      .arg(secs, 2, 10, QLatin1Char('0'));
}

// Начинает новую игру, генерирует доску судоку с заданной сложностью.
void SudokuController::newGame(int difficulty) {
  board_->clear(); // Очищаем доску
  board_->clearOriginals(); // Сбрасываем метки исходных чисел
  generator_.generate(*board_, difficulty);

  // Вместо view_->setBoard(*board_) обновляем ячейки вручную
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      view_->UpdateCell(row, col, board_->getCellValue(row, col));
      view_->getCell(row, col)->setOriginal(board_->isCellOriginal(row, col));
    }
  }

  // Инициализация параметров игры
  gameStarted_ = true;
  secondsElapsed_ = 0;
  errorCount_ = 0;

  // запускаем таймер
  startTimer();

  // передаём ошибки и время в MainWindow
  view_->updateGameStats(formatTime(secondsElapsed_), errorCount_);

  // сброс выбранной ячейки
  selectedRow_ = -1;
  selectedCol_ = -1;

  view_->show();
}

void SudokuController::startTimer() {
  if (timer_) {
    timer_->start(1000); // Обновление каждую секунду
  }
}

void SudokuController::stopTimer() {
  if (timer_) {
    timer_->stop();
  }
}

void SudokuController::updateTimer() {
  secondsElapsed_++;
  view_->updateTimerDisplay(secondsElapsed_); // Обновляем UI
}

// обработка введенного числа
void SudokuController::onNumberEntered(int row, int col, int value) {
  if (board_->isCellOriginal(row, col)) {
    QMessageBox::warning(view_, "Ошибка", "Нельзя изменять исходные цифры!");
    view_->UpdateCell(row, col, board_->getCellValue(row, col));
    return;
  }

  // Добавляем проверку
  if (value != 0 && !board_->isValidMove(row, col, value)) {
    QMessageBox::warning(view_, "Ошибка", "Недопустимое значение!");
    errorCount_++;
    view_->updateErrorDisplay(errorCount_);
    view_->UpdateCell(row, col, board_->getCellValue(row, col));
    return;
  }

  // Если всё ок — обновляем доску
  board_->setCellValue(row, col, value);
  board_->setCellOriginal(row, col, false);

  SudokuCell *cell = view_->getCell(row, col);
  if (cell) {
    cell->setOriginal(false);
    cell->setDisplayValue(value);
  }

  // Обновляем подсветку после изменения значения
  // if (selectedRow_ == row && selectedCol_ == col) {
  //   selectedRow_ = row;
  //   selectedCol_ = col;
  // }
  view_->refreshHighlight();

  checkSolved();
} // проверка допустимости введенного числа
bool SudokuController::checkInput(int row, int col, int value) {
  return board_->isValidMove(row, col, value);
}

// проверка полностью решенного судоку
void SudokuController::checkSolved() {
  if (board_->isSolved()) {
    QMessageBox::information(view_, tr("Поздравляем!"),
                             tr("Вы решили судоку!"));
    gameStarted_ = false; // игра завершена
    stopTimer();          // остановить таймер

    emit gameFinished(); // испускаем сигнал
  }
}

void SudokuController::onGameSelected(int difficulty) {
  newGame(difficulty); // Начинаем новую игру с выбранной сложностью
}

void SudokuController::handleCellInteraction(int row, int col) {
  if (!gameStarted_) return;

  // Запоминаем выбранную ячейку
  selectedRow_ = row;
  selectedCol_ = col;
}

// Обработчик ввода (вызывается при двойном клике)
void SudokuController::processCellInput(int row, int col) {
  if (!gameStarted_) return;

  bool ok;
  int value = QInputDialog::getInt(view_, tr("Введите число"),
                                 tr("Число (1-9, 0 для очистки:"),
                                 0, 0, 9, 1, &ok);

  if (ok) {
    onNumberEntered(row, col, value);
  }
}
