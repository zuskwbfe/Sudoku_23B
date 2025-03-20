#pragma once
#include "SudokuCell.h"
#include <QGridLayout>
#include <QMainWindow>

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr);
  void UpdateCell(int row, int col, int value);

private:
  QGridLayout *gridLayout;
  SudokuCell *cells[9][9];
};
