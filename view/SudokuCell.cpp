#include "SudokuCell.h"

SudokuCell::SudokuCell(int r, int c, QWidget *parent)
    : QPushButton(parent), row(r), col(c) {
  // Устанавливаем фиксированный размер ячейки 40x40 пикселей
  setFixedSize(40, 40);
  // Подключаем обработчик клика
  connect(this, &QPushButton::clicked,
          [this]() { emit cellClicked(row, col); });
}

// Установка отображаемого значения в ячейке
void SudokuCell::setDisplayValue(int value) {
  setText(value > 0 ? QString::number(value) : "");
}
