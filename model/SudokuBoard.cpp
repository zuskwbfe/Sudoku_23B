#include "SudokuBoard.h"
#include <algorithm>

SudokuBoard::SudokuBoard() : board_{0} {}

int SudokuBoard::getCellValue(int row, int col) const {
  return board_[row][col];
}

// Установка значения в клетку
void SudokuBoard::setCellValue(int row, int col, int value) {
  board_[row][col] = value;
}

// Проверка допустимости хода
bool SudokuBoard::isValidMove(int row, int col, int value) const {
  if (value < 0 || value > 9) {
    return false;
  }

  if (value == 0)
    return true; // Всегда разрешаем очистку клетки (установку 0)

  // Проверка строки на дублирование числа
  for (int c = 0; c < 9; ++c) {
    if (board_[row][c] == value && c != col) {
      return false;
    }
  }

  // Проверка столбца на дублирование числа
  for (int r = 0; r < 9; ++r) {
    if (board_[r][col] == value && r != row) {
      return false;
    }
  }

  // Проверка подквадрата 3x3 на дублирование числа
  int subgridRowStart = row - row % 3;
  int subgridColStart = col - col % 3;
  for (int r = subgridRowStart; r < subgridRowStart + 3; ++r) {
    for (int c = subgridColStart; c < subgridColStart + 3; ++c) {
      if (board_[r][c] == value && (r != row || c != col)) {
        return false;
      }
    }
  }

  return true; // Ход допустим
}

// Проверка полностью решённого судоку
bool SudokuBoard::isSolved() const {
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      if (board_[row][col] == 0 || !isValidMove(row, col, board_[row][col]))
        return false;
    }
  }
  return true; // Все клетки заполнены корректно
}

// Реализация Iterator
int SudokuBoard::Iterator::operator*() const {
  return board_->getCellValue(row_, col_);
}

// Переход к следующей клетке
SudokuBoard::Iterator &SudokuBoard::Iterator::operator++() {
  ++col_;
  if (col_ == 9) {
    col_ = 0;
    ++row_;
  }
  return *this;
}

// Проверка на неравенство итераторов
bool SudokuBoard::Iterator::operator!=(const Iterator &other) const {
  return row_ != other.row_ || col_ != other.col_;
}

bool SudokuBoard::isCellOriginal(int row, int col) const {
  return originalCells_[row][col];
}

void SudokuBoard::setCellOriginal(int row, int col, bool original) {
  originalCells_[row][col] = original;
}

void SudokuBoard::clearOriginals() {
  for (auto &row : originalCells_) {
    row.fill(false);
  }
}

void SudokuBoard::clear() {
  for (auto &row : board_) {
    std::fill(row.begin(), row.end(), 0);
  }
  clearOriginals();
}
