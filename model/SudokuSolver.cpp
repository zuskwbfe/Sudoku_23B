#include "SudokuSolver.h"

bool BacktrackingSolver::solve(SudokuBoard& board) {
    return solveRecursive(board);
}

bool BacktrackingSolver::solveRecursive(SudokuBoard& board) {
    if (board.isSolved()){return true;}
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board.getCellValue(row, col) == 0) { // Ищем пустую клетку
                for (int num = 1; num <= 9; ++num) {
                    if (board.isValidMove(row, col, num)) { // Проверяем можно ли поставить число
                        board.setCellValue(row, col, num); // Ставим число

                        if (solveRecursive(board)) {
                            return true; // Верное решение
                        } else {
                            board.setCellValue(row, col, 0); // Откатываем назад и пробуем другое число
                        }
                    }
                }
                return false; // Нет подходящих чисел => возвращаемся назад
            }
        }
    }
    return true; // Судоку решено
}
