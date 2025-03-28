#pragma once

#include "SudokuBoard.h"
#include "SudokuSolver.h"

class SudokuGenerator {
    public:
    SudokuGenerator(SudokuSolver* solver);
    void generate(SudokuBoard& board, int difficutly);

    private:
    SudokuSolver* solver_;

};
