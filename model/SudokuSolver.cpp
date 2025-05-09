#include "SudokuSolver.h"
#include <algorithm>
#include <vector>

bool BacktrackingSolver::solve(SudokuBoard &board) {
  return solveRecursive(board);
}

bool BacktrackingSolver::randomizedSolve(SudokuBoard &board,
                                         std::mt19937 &gen) {
  return solveRecursive(board, true, &gen);
}

bool BacktrackingSolver::solveRecursive(SudokuBoard &board, bool randomize,
                                        std::mt19937 *gen) {
  if (board.isSolved())
    return true;

  int row, col;
  if (!findEmptyCell(board, row, col))
    return false;

  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  if (randomize && gen) {
    std::shuffle(numbers.begin(), numbers.end(), *gen);
  }

  for (int num : numbers) {
    if (board.isValidMove(row, col, num)) {
      board.setCellValue(row, col, num);
      if (solveRecursive(board, randomize, gen)) {
        return true;
      }
      board.setCellValue(row, col, 0);
    }
  }
  return false;
}

bool BacktrackingSolver::findEmptyCell(SudokuBoard &board, int &row, int &col) {
  for (row = 0; row < 9; row++) {
    for (col = 0; col < 9; col++) {
      if (board.getCellValue(row, col) == 0) {
        return true;
      }
    }
  }
  return false;
}
