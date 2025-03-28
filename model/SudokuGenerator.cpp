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
    // Удаление номеров в зависимости от сложности
    std::random_device rd;
    std::mt19937 g(rd()); // Генератор случайных чисел
    std::vector<std::pair<int, int>> cells; // Вектор для хранения координат клеток
    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 9; j++){
        cell.push_back({i, j}); // Добавляем все 81 клетку
    }}

    int numRemove = 0;
    if (difficutly == 0){
        numRemove = 40;
    } else if (difficutly == 1){
        numRemove = 50;
    } else {
        numRemove = 60;
    }

    int removeCount = 0;
    for (const auto& cell : cells) {
        if (removeCount >= numRemove){
            break; // Достаточно удалили
        }
        int row = cell.first;
        int col = cell.second;
        int originalValue = board.geteCellValue(row, col);
        board.setCellValue(row, col, 0); // Удаляем число (записываем 0)

        SudokuBoard tempBoard = board; // Копия доски
        int solutions = 0;


}
