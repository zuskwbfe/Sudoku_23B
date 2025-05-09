#include "SudokuCell.h"
#include <QColor>

SudokuCell::SudokuCell(int r, int c, QWidget *parent)
    : QPushButton(parent), row(r), col(c) {
  setFixedSize(40, 40);
  setStyleSheet("font-size: 20px;"); // Базовый стиль
  connect(this, &QPushButton::clicked,
          [this]() { emit cellClicked(row, col); });
}

// Установка отображаемого значения в ячейке
void SudokuCell::setDisplayValue(int value) {
  setText(value > 0 ? QString::number(value) : "");

  QPalette p = palette();
  if (value > 0) {
    p.setColor(QPalette::ButtonText, isOriginal_ ? Qt::black : Qt::blue);
  } else {
    p.setColor(QPalette::ButtonText, Qt::black); // или Qt::gray, если хочешь
  }
  setPalette(p);
}

void SudokuCell::setOriginal(bool isOriginal) {
  isOriginal_ = isOriginal;
  update(); // Триггер перерисовки
}
