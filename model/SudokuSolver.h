#pragma once

#include "SudokuBoard.h"

class SudokuSolver {
public:
    // Стратегия (паттерн Стратегия) - интерфейс для алгоритмов решения
    virtual bool solve(SudokuBoard& board) = 0;
    virtual ~SudokuSolver() = default;
};


// Конкретная стратегия: Backtracking (поиск с возвратом)
// Наследуется от SudokuSolver
class BacktrackingSolver : public SudokuSolver {
public:
    bool solve(SudokuBoard& board) override;

private:
    bool solveRecursive(SudokuBoard& board);
};
