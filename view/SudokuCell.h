#pragma once
#include <QPushButton>

class SudokuCell : public QPushButton {
  Q_OBJECT
public:
  SudokuCell(int row, int col, QWidget *parent = nullptr);
  void setDisplayValue(int value);
  void setOriginal(bool isOriginal);
  bool isOriginal() const { return isOriginal_; }
  int GetRow() const { return row; }
  int GetCol() const { return col; }

signals:
  // Сигнал клика на ячейку
  void cellClicked(int row, int col);

private:
  int row;
  int col;
  QColor originalColor = Qt::black;
  QColor userColor = QColor(31, 174, 233);
  bool isOriginal_ = false;
};
