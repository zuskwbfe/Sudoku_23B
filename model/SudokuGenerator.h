#pragma once
#include "SudokuBoard.h"
#include "SudokuSolver.h"
#include <memory>
#include <random>
#include <vector>

// генератор судоку с возможностью настройки сложности
class SudokuGenerator {
public:
  SudokuGenerator(SudokuSolver *solver);
  // генерирует новую доску с заданной сложностью
  void generate(SudokuBoard &board, int difficulty);

private:
    void removeCells(SudokuBoard& board, int maxRemove);
    bool hasUniqueSolution(SudokuBoard& board);
    bool findDifferentSolution(SudokuBoard& board, const SudokuBoard& firstSolution);
    SudokuSolver* solver_;
    std::mt19937 gen_;
};
