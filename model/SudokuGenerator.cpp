#include "SudokuGenerator.h"
#include "SudokuSolver.h"
#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>

SudokuGenerator::SudokuGenerator(SudokuSolver *solver)
    : solver_(solver), gen_(std::random_device{}()) {}

void SudokuGenerator::generate(SudokuBoard &board, int difficulty) {
  board.clear();
  board.clearOriginals();

  // Генерация случайной решенной доски
  if (!solver_->randomizedSolve(board, gen_)) {
    throw std::runtime_error("Failed to generate solved board");
  }

  // Настройка сложности
  int numToRemove = 40 + difficulty * 5; // Более плавная градация
  removeCells(board, numToRemove);

  // 3. Помечаем ВСЕ оставшиеся цифры как оригинальные
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      // Если клетка не пустая - она оригинальная
      if (board.getCellValue(row, col) != 0) {
        board.setCellOriginal(row, col, true);
      }
    }
  }
}

void SudokuGenerator::removeCells(SudokuBoard &board, int numToRemove) {
  std::vector<std::pair<int, int>> cells;
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      cells.emplace_back(i, j);
    }
  }

  std::shuffle(cells.begin(), cells.end(), gen_);
  int removed = 0;

  for (const auto &[row, col] : cells) {
    if (removed >= numToRemove)
      break;

    int saved = board.getCellValue(row, col);
    if (saved == 0)
      continue;

    board.setCellValue(row, col, 0);

    if (!hasUniqueSolution(board)) {
      board.setCellValue(row, col, saved);
    } else {
      removed++;
    }
  }
}

bool SudokuGenerator::hasUniqueSolution(SudokuBoard &board) {
  int count = 0;
  countSolutions(board, count);
  return count == 1;
}

void SudokuGenerator::countSolutions(SudokuBoard &board, int &solutionCount,
                                     int depth) {
  if (solutionCount >= 2)
    return;

  // Оптимизация: находим клетку с наименьшим количеством вариантов
  int bestRow = -1, bestCol = -1;
  std::vector<int> bestNumbers;

  // Поиск оптимальной клетки для ветвления
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      if (board.getCellValue(row, col) != 0)
        continue;

      std::vector<int> possible;
      for (int num = 1; num <= 9; ++num) {
        if (board.isValidMove(row, col, num)) {
          possible.push_back(num);
        }
      }

      if (possible.empty())
        return;

      if (bestRow == -1 || possible.size() < bestNumbers.size()) {
        bestRow = row;
        bestCol = col;
        bestNumbers = possible;
      }
    }
  }

  if (bestRow == -1) {
    solutionCount++;
    return;
  }

  // Перебор чисел в случайном порядке
  std::shuffle(bestNumbers.begin(), bestNumbers.end(), gen_);

  for (int num : bestNumbers) {
    board.setCellValue(bestRow, bestCol, num);
    countSolutions(board, solutionCount, depth + 1);
    board.setCellValue(bestRow, bestCol, 0);

    if (solutionCount >= 2)
      break;
  }
}
