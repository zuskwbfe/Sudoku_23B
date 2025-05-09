#pragma once
#include "SudokuBoard.h"
#include "SudokuSolver.h"
#include <random>

// генератор судоку с возможностью настройки сложности
class SudokuGenerator {
public:
  explicit SudokuGenerator(SudokuSolver *solver);
  // генерирует новую доску с заданной сложностью
  void generate(SudokuBoard &board, int difficulty);

private:
  void removeCells(SudokuBoard &board, int numToRemove);
  bool hasUniqueSolution(SudokuBoard &board);
  void countSolutions(SudokuBoard &board, int &solutionCount, int depth = 0);
  SudokuSolver *solver_;
  std::mt19937 gen_;
};
