#pragma once

#include <array>
#include <vector>

// Класс, представляющий доску для игры в Судоку
class SudokuBoard {
public:
  SudokuBoard();

  int getCellValue(int row, int col) const;
  void setCellValue(int row, int col, int value);
  bool isValidMove(int row, int col, int value) const;

  bool isSolved() const;
  void clear();

  bool isCellOriginal(int row, int col) const;
  void setCellOriginal(int row, int col, bool original);
  void clearOriginals();

  void setSolution(const SudokuBoard &solvedBoard);

  void copyFrom(const SudokuBoard &source);
  bool equals(const SudokuBoard &other) const;

  void setNote(int row, int col, int value, bool on);
  bool hasNote(int row, int col, int value) const;
  void clearNotes(int row, int col);
  void clearAllNotes();

  // Iterator (паттерн Итератор)
  class Iterator {
  public:
    Iterator(SudokuBoard *board, int row, int col)
        : board_(board), row_(row), col_(col) {}

    // Оператор разыменования - получить значение текущей клетки
    int operator*() const;
    // Префиксный инкремент - переход к следующей клетке
    Iterator &operator++();
    // Проверка на неравенство итераторов
    bool operator!=(const Iterator &other) const;

  private:
    SudokuBoard *board_;
    int row_;
    int col_;
  };

  Iterator begin() { return Iterator(this, 0, 0); } // Итератор на начало доски
  Iterator end() { return Iterator(this, 9, 0); } // Итератор на конец доски

private:
  // Внутреннее представление доски - двумерный массив 9x9
  std::array<std::array<int, 9>, 9> board_;
  std::array<std::array<bool, 9>, 9> originalCells_ = {}; // Новое поле
  std::array<std::array<int, 9>, 9> solution_;
  std::array<std::array<std::array<bool, 9>, 9>, 9>
      notes_; // Пометки для каждой ячейки
  bool hasSolution_ = false;
};
