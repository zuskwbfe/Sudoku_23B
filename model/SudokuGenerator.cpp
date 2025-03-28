#include "SudokuGenerator.h"
#include <random>
#include <algorithm>
#include <vector>

SudokuGenerator::SudokuGenerator(SudokuSolver* solver): solver_(solver){}

// Очищает переданную доску (заполняет её нулями или пустыми значениями).
void SudokuGenerator::generate(SudokuBoard& board, int difficutly){
    board.clear();

    // Создайние полностью решенной доски
    solver_-> solver(board);
