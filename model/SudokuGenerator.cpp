#include "SudokuGenerator.h"
#include "SudokuSolver.h"
#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>

SudokuGenerator::SudokuGenerator(SudokuSolver *solver) : solver_(solver) {}

void SudokuGenerator::generate(SudokuBoard &board, int difficulty) {
  board.clear();

  // создаем решенную доску
  if (!solver_->solve(board)) {
    throw std::runtime_error("Failed to generate solved board");
  }

  // подготовка клеток для удаления
  std::vector<std::pair<int, int>> cells;
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      cells.emplace_back(i, j);
    }
  }

  std::random_device rd; // создаем генератор случайных чисел
  // перемешиваем вектор клеток для случайного порядка удаления чисел
  std::shuffle(cells.begin(), cells.end(), std::mt19937(rd()));

  // количество удаляемых чисел
  int numToRemove = 40 + difficulty * 10; // 40, 50 или 60

  // удаление чисел
  int removed = 0;
  for (const auto &[row, col] : cells) {
    if (removed >= numToRemove)
      break;

    int savedValue = board.getCellValue(row, col);
    if (savedValue == 0)
      continue;

    board.setCellValue(row, col, 0);

    // упрощенная проверка уникальности
    SudokuBoard tempBoard = board;
    int solutions = 0;
    countSolutions(tempBoard, solutions, 0);

    if (solutions == 1) {
      removed++;
    } else {
      board.setCellValue(row, col, savedValue);
    }
  }
}

void SudokuGenerator::countSolutions(SudokuBoard &board, int &solutionCount,
                                     int depth) {
  if (solutionCount > 1)
    return; // прекращаем если нашли более 1 решения

  // находим первую пустую клетку
  int row = -1, col = -1;
  for (int i = 0; i < 9 && row == -1; ++i) {
    for (int j = 0; j < 9 && col == -1; ++j) {
      if (board.getCellValue(i, j) == 0) {
        row = i;
        col = j;
      }
    }
  }

  // если нет пустых клеток - решение найдено
  if (row == -1) {
    solutionCount++;
    return;
  }

  // перебираем возможные числа
  for (int num = 1; num <= 9 && solutionCount <= 1; ++num) {
    if (board.isValidMove(row, col, num)) {
      board.setCellValue(row, col, num);
      countSolutions(board, solutionCount, depth + 1);
      board.setCellValue(row, col, 0);
    }
  }
}
