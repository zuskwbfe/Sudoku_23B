#include "SudokuGenerator.h"
#include <algorithm>
#include <stdexcept>

SudokuGenerator::SudokuGenerator(SudokuSolver *solver)
    : solver_(solver), gen_(std::random_device{}()) {}

void SudokuGenerator::generate(SudokuBoard &board, int difficulty) {
    board.clear();
    board.clearOriginals();

    // Создаем временную доску для решения
    SudokuBoard solvedBoard;
    solvedBoard.copyFrom(board);

    // Генерируем полное решение
    if (!solver_->randomizedSolve(solvedBoard, gen_)) {
        throw std::runtime_error("Failed to generate solved board");
    }

    // Сохраняем решение в исходной доске
    board.setSolution(solvedBoard);
    board.copyFrom(solvedBoard);

    // Удаляем ячейки
    const int maxRemove = std::min(55, 35 + difficulty * 5);
    removeCells(board, maxRemove);

    // Помечаем оригинальные ячейки
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board.getCellValue(row, col) != 0) {
                board.setCellOriginal(row, col, true);
            }
        }
    }
}
void SudokuGenerator::removeCells(SudokuBoard &board, int maxRemove) {
  std::vector<std::pair<int, int>> cells;

  for (int r = 0; r < 9; ++r) {
    for (int c = 0; c < 9; ++c) {
      cells.emplace_back(r, c);
    }
  }

  std::shuffle(cells.begin(), cells.end(), gen_);
  int removed = 0;

  for (const auto &[row, col] : cells) {
    if (removed >= maxRemove)
      break;

    int original = board.getCellValue(row, col);
    if (original == 0)
      continue;

    board.setCellValue(row, col, 0);

    if (hasUniqueSolution(board)) {
      ++removed;
    } else {
      board.setCellValue(row, col, original);
    }
  }
}

bool SudokuGenerator::hasUniqueSolution(SudokuBoard &board) {
  SudokuBoard solution1;
  solution1.copyFrom(board);

  // Находим первое решение
  if (!solver_->solve(solution1)) {
    return false; // Нет решений
  }

  // Пытаемся найти решение, отличное от первого
  return !findDifferentSolution(board, solution1);
}

bool SudokuGenerator::findDifferentSolution(SudokuBoard &board,
                                            const SudokuBoard &firstSolution) {
  // Создаем копию для безопасной работы
  SudokuBoard workBoard;
  workBoard.copyFrom(board);

  int row = -1, col = -1;

  // Находим первую пустую ячейку
  for (int r = 0; r < 9 && row == -1; r++) {
    for (int c = 0; c < 9; c++) {
      if (workBoard.getCellValue(r, c) == 0) {
        row = r;
        col = c;
        break;
      }
    }
  }

  if (row == -1)
    return false; // Нет пустых ячеек

  // Получаем значение из первого решения
  int firstSolutionValue = firstSolution.getCellValue(row, col);

  // Перебираем все возможные значения, кроме значения из первого решения
  for (int num = 1; num <= 9; num++) {
    if (num == firstSolutionValue)
      continue;

    if (workBoard.isValidMove(row, col, num)) {
      workBoard.setCellValue(row, col, num);

      SudokuBoard testSolution;
      testSolution.copyFrom(workBoard);

      if (solver_->solve(testSolution)) {
        // Проверяем, отличается ли решение от первого
        for (int r = 0; r < 9; r++) {
          for (int c = 0; c < 9; c++) {
            if (testSolution.getCellValue(r, c) !=
                firstSolution.getCellValue(r, c)) {
              return true; // Найдено другое решение
            }
          }
        }
      }
      workBoard.setCellValue(row, col, 0);
    }
  }

  return false; // Другое решение не найдено
}
