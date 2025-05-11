#include "SudokuGenerator.h"
#include "SudokuSolver.h"
#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>


SudokuGenerator::SudokuGenerator(SudokuSolver* solver)
  : solver_(solver), gen_(std::random_device{}()) {}

void SudokuGenerator::generate(SudokuBoard& board, int difficulty) {
  board.clear();
  board.clearOriginals();

  // Шаг 1: создаем случайно решенную доску
  if (!solver_->randomizedSolve(board, gen_)) {
    throw std::runtime_error("Failed to generate solved board");
  }

  // Шаг 2: удаляем ячейки, сохраняя уникальное решение
  const int maxRemove = std::min(55, 35 + difficulty * 5);
  removeCells(board, maxRemove);

  // Шаг 3: помечаем оставшиеся как оригинальные
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      if (board.getCellValue(row, col) != 0) {
        board.setCellOriginal(row, col, true);
      }
    }
  }
}

void SudokuGenerator::removeCells(SudokuBoard& board, int maxRemove) {
  std::vector<std::pair<int, int>> cells;

  for (int r = 0; r < 9; ++r) {
    for (int c = 0; c < 9; ++c) {
      cells.emplace_back(r, c);
    }
  }

  std::shuffle(cells.begin(), cells.end(), gen_);
  int removed = 0;

  for (const auto& [row, col] : cells) {
    if (removed >= maxRemove) break;

    int original = board.getCellValue(row, col);
    if (original == 0) continue;

    board.setCellValue(row, col, 0);

    if (hasUniqueSolution(board)) {
      ++removed;
    } else {
      board.setCellValue(row, col, original);
    }
  }
}

bool SudokuGenerator::hasUniqueSolution(SudokuBoard& board) {
  int count = 0;
  countSolutions(board, count);
  return count == 1;
}

void SudokuGenerator::countSolutions(SudokuBoard& board, int& count) {
  if (count >= 2) return;

  int bestRow = -1, bestCol = -1;
  std::vector<int> bestOptions;

  // Найти ячейку с наименьшим числом возможных значений
  for (int r = 0; r < 9; ++r) {
    for (int c = 0; c < 9; ++c) {
      if (board.getCellValue(r, c) != 0) continue;

      std::vector<int> options;
      for (int n = 1; n <= 9; ++n) {
        if (board.isValidMove(r, c, n)) {
          options.push_back(n);
        }
      }

      if (options.empty()) return; // тупик

      if (bestOptions.empty() || options.size() < bestOptions.size()) {
        bestRow = r;
        bestCol = c;
        bestOptions = std::move(options);
      }
    }
  }

  if (bestRow == -1) {
    ++count; // Решение найдено
    return;
  }

  std::shuffle(bestOptions.begin(), bestOptions.end(), gen_);

  for (int num : bestOptions) {
    board.setCellValue(bestRow, bestCol, num);
    countSolutions(board, count);
    board.setCellValue(bestRow, bestCol, 0);
    if (count >= 2) return;
  }
}
