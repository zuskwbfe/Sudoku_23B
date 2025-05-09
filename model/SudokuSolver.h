#pragma once

#include "SudokuBoard.h"
#include <random>

class SudokuSolver {
public:
  virtual bool solve(SudokuBoard &board) = 0;
  virtual bool randomizedSolve(SudokuBoard &board, std::mt19937 &gen) = 0;
  virtual ~SudokuSolver() = default;
};

// Конкретная стратегия: Backtracking (поиск с возвратом)
// Наследуется от SudokuSolver
class BacktrackingSolver : public SudokuSolver {
public:
  bool solve(SudokuBoard &board) override;
  bool randomizedSolve(SudokuBoard &board, std::mt19937 &gen) override;
  bool findEmptyCell(SudokuBoard &board, int &row, int &col);

private:
  bool solveRecursive(SudokuBoard &board, bool randomize = false,
                      std::mt19937 *gen = nullptr);
};
