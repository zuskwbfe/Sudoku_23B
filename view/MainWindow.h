#pragma once
#include "../model/SudokuBoard.h"
#include "SudokuCell.h"
#include <QGridLayout>
#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);
  // Обновить значение и отображение ячейки
  void UpdateCell(int row, int col, int value);

  SudokuBoard &getBoard() { return board; } // Получение доступа к модели доски
  void updateBoard(); // Обновление отображения всей доски

private:
  // Сетка для размещения ячеек
  QGridLayout *gridLayout;
  SudokuCell *cells[9][9];
  SudokuBoard board;

signals:
  // Сигнал при клике на ячейку
  void CellClicked(int row, int col);
  // Сигнал при вводе числа в ячейку
  void numberEntered(int row, int col, int value);
};
