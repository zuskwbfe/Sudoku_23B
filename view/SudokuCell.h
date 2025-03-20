#pragma once
#include <QPushButton>

class SudokuCell : public QPushButton {
  Q_OBJECT
public:
  SudokuCell(int row, int col, QWidget *parent = nullptr);
  void setDisplayValue(int value);
  int row() const { return _row; }
  int col() const { return _col; }

signals:
  void cellClicked(int row, int col);

private:
  int _row, _col;
};
