#include "SudokuCell.h"

SudokuCell::SudokuCell(int row, int col, QWidget *parent)
    : QPushButton(parent), _row(row), _col(col) {
  setFixedSize(40, 40);
  connect(this, &QPushButton::clicked,
          [this]() { emit cellClicked(_row, _col); });
}

void SudokuCell::setDisplayValue(int value) {
  setText(value > 0 ? QString::number(value) : "");
}
