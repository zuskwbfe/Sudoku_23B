#pragma once
#include "SudokuBoard.h"
#include "SudokuSolver.h"

// генератор судоку с возможностью настройки сложности
class SudokuGenerator {
public:
  explicit SudokuGenerator(SudokuSolver *solver);
  // генерирует новую доску с заданной сложностью
  void generate(SudokuBoard &board, int difficutly);

  // подсчитывает количество возможных решений для доски
  void countSolutions(SudokuBoard &board, int &solutionCount, int depth = 0);

private:
  SudokuSolver *solver_;
};
