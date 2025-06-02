#include "SudokuController.h"
#include "SudokuGenerator.h"
#include "GameRecords.h"
#include <QInputDialog> // диалог ввода чисел
#include <QMessageBox>  // всплывающие сообщения

SudokuController::SudokuController(SudokuBoard *board, MainWindow *view,
                                   QObject *parent)
    : QObject(parent), board_(board), view_(view), solver_(),
      generator_(&solver_), selectedRow_(-1), selectedCol_(-1),
      gameStarted_(false) {
  // соединение сигнала клика по ячейке со слотом обработки
  connect(view_, &MainWindow::CellClicked, this,
          &SudokuController::handleCellInteraction);
  connect(view_, &MainWindow::CellDoubleClicked, this,
          &SudokuController::processCellInput);

  // Инициализация таймера
  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &SudokuController::updateTimer);
  records.loadFromFile(); // Загружаем рекорды
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
  currentDifficulty_ = static_cast<Difficulty>(difficulty);
  board_->clear(); // Очищаем доску
  board_->clearOriginals(); // Сбрасываем метки исходных чисел
  generator_.generate(*board_, difficulty);

  // Вместо view_->setBoard(*board_) обновляем ячейки вручную
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      view_->UpdateCell(row, col, board_->getCellValue(row, col));
      view_->getCell(row, col)->setOriginal(board_->isCellOriginal(row, col));
      view_->getCell(row, col)->clearNotes();
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
  if (noteMode_) {
    handleNote(row, col, value);
    return;
  }
  if (board_->isCellOriginal(row, col)) {
    QMessageBox::warning(view_, "Ошибка", "Нельзя изменять исходные цифры!");
    view_->UpdateCell(row, col, board_->getCellValue(row, col));
    return;
  }

  // Добавляем проверку
  if (value != 0 && !board_->isValidMove(row, col, value)) {
    errorCount_++;
    view_->updateErrorDisplay(errorCount_);

    // Проверяем, не превысили ли лимит ошибок
    if (errorCount_ >= MAX_ERRORS) {
      checkGameOver();
    } else {
      QMessageBox::warning(view_, "Ошибка", "Недопустимое значение!");
    }

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

  if (value != 0) {
    clearNotesInArea(row, col, value);
  }

  view_->refreshHighlight();

  checkSolved();
} // проверка допустимости введенного числа

bool SudokuController::checkInput(int row, int col, int value) {
  return board_->isValidMove(row, col, value);
}

// проверка полностью решенного судоку
void SudokuController::checkSolved() {
    if (board_->isSolved()) {
        // Запрашиваем имя игрока
        bool ok;
        QString playerName = QInputDialog::getText(view_, tr("Поздравляем!"),
            tr("Вы решили судоку!\nВведите ваше имя для таблицы рекордов:"), QLineEdit::Normal, "Player", &ok);

        if (!ok || playerName.isEmpty()) {
            playerName = "Player";
        }

        // Создаем запись
        GameRecords::Record newRecord;
        newRecord.playerName = playerName.toStdString();
        newRecord.timeSeconds = secondsElapsed_;
        newRecord.level = currentDifficulty_;

        // Добавляем запись
        records.addRecord(newRecord);

        // Останавливаем игру
        gameStarted_ = false;
        stopTimer();
        emit gameCompleted();
    }
}

void SudokuController::onGameSelected(int difficulty) {
  newGame(difficulty); // Начинаем новую игру с выбранной сложностью
}

void SudokuController::handleCellInteraction(int row, int col) {
    if (!gameStarted_) return;

    selectedRow_ = row;
    selectedCol_ = col;

    // Снимаем предыдущую подсветку
    if (lastHighlightedDigit_ != 0) {
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (board_->hasNote(r, c, lastHighlightedDigit_)) {
                    SudokuCell* cell = view_->getCell(r, c);
                    if (cell) {
                        cell->setHighlightedNote(lastHighlightedDigit_, false);
                    }
                }
            }
        }
    }

    // Устанавливаем новую подсветку
    int digit = board_->getCellValue(row, col);
    lastHighlightedDigit_ = 0;

    if (digit != 0) {
        lastHighlightedDigit_ = digit;
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (board_->hasNote(r, c, digit)) {
                    SudokuCell* cell = view_->getCell(r, c);
                    if (cell) {
                        cell->setHighlightedNote(digit, true);
                    }
                }
            }
        }
    }
}

// Обработчик ввода (вызывается при двойном клике)
void SudokuController::processCellInput(int row, int col) {
  if (!gameStarted_)
    return;

  bool ok;
  int value =
      QInputDialog::getInt(view_, tr("Введите число"),
                           tr("Число (1-9, 0 для очистки:"), 0, 0, 9, 1, &ok);

  if (ok) {
    onNumberEntered(row, col, value);
  }
}

void SudokuController::handleNote(int row, int col, int value) {
  if (!gameStarted_ || board_->isCellOriginal(row, col) ||
      board_->getCellValue(row, col) != 0) {
    return;
  }

  bool currentState = board_->hasNote(row, col, value);
  board_->setNote(row, col, value, !currentState);

  SudokuCell *cell = view_->getCell(row, col);
  if (cell) {
    cell->setNote(value, !currentState);
  }
}

void SudokuController::clearNotesInArea(int row, int col, int value) {
  if (!gameStarted_ || value < 1 || value > 9)
    return;

  // Очистка в строке
  for (int c = 0; c < 9; ++c) {
    if (c != col) { // Пропускаем текущую ячейку
      clearNoteInCell(row, c, value);
    }
  }

  // Очистка в столбце
  for (int r = 0; r < 9; ++r) {
    if (r != row) { // Пропускаем текущую ячейку
      clearNoteInCell(r, col, value);
    }
  }

  // Очистка в блоке 3x3
  int blockRowStart = row - row % 3;
  int blockColStart = col - col % 3;
  for (int r = blockRowStart; r < blockRowStart + 3; ++r) {
    for (int c = blockColStart; c < blockColStart + 3; ++c) {
      if (r != row || c != col) { // Пропускаем текущую ячейку
        clearNoteInCell(r, c, value);
      }
    }
  }
}

void SudokuController::clearNoteInCell(int row, int col, int value) {
  // Очищаем пометку в модели
  board_->setNote(row, col, value, false);

  // Обновляем отображение ячейки
  SudokuCell *cell = view_->getCell(row, col);
  if (cell) {
    cell->setNote(value, false);
    cell->update(); // Принудительное обновление
  }
}

void SudokuController::checkGameOver() {
  if (errorCount_ >= MAX_ERRORS) {
    gameStarted_ = false;
    stopTimer();

    // Показываем сообщение о завершении игры
    QMessageBox gameOverBox;
    gameOverBox.setWindowTitle("Игра завершена");
    gameOverBox.setText("Вы совершили 3 ошибки! Игра завершена.");
    gameOverBox.setStandardButtons(QMessageBox::Ok);
    gameOverBox.exec();

    // Испускаем сигнал завершения игры
    emit gameCompleted();
  }
}
