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
  connect(view_, &MainWindow::CellClicked, this,
          &SudokuController::onCellClicked);


  // Инициализация таймера
  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &SudokuController::updateTimer);


  newGame(0);
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
  view_->setBoard(*board_);

  // Инициализация параметров игры
  gameStarted_ = true;
  secondsElapsed_ = 0;
  errorCount_ = 0;

  // запускаем таймер
  startTimer();

  // передаём ошибки и время в MainWindow
  view_->updateGameStats(formatTime(secondsElapsed_), errorCount_);

  // обновление всех ячеек на представлении
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      view_->UpdateCell(row, col, board_->getCellValue(row, col));
    }
  }

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

// слот обработки клика по ячейке
void SudokuController::onCellClicked(int row, int col) {
  if (!gameStarted_)
    return; // если игра не начата - игнорируем

  // запоминаем выбранную ячейку
  selectedRow_ = row;
  selectedCol_ = col;

  // отображаем диалог ввода числа
  bool ok;
  int value = QInputDialog::getInt(view_, tr("Введите число"),
                                   tr("Число (1-9, 0 для очистки):"),
                                   0, // значение по умолчанию
                                   0, // минимум
                                   9, // максимум
                                   1, // шаг
                                   &ok // флаг подтверждения ввода
  );

  // если пользователь подтвердил ввод
  if (ok) {
    onNumberEntered(row, col, value);
  }
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

    emit gameFinished();  // испускаем сигнал
  }
}


void SudokuController::onGameSelected(int difficulty) {
    newGame(difficulty);  // Начинаем новую игру с выбранной сложностью
}
