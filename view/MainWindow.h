#pragma once
#include "../model/SudokuBoard.h"
#include "SudokuCell.h"
#include <QGridLayout>
#include <QLabel> // Для отображения таймера и ошибок
#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);
  // обновить значение и отображение ячейки
  void UpdateCell(int row, int col, int value);

  void updateTimerDisplay(int seconds);
  void updateErrorDisplay(int errors);
  void updateGameStats(const QString &time,
                       int errors); // Комбинированный метод
  SudokuCell *getCell(int row, int col);
  SudokuBoard &getBoard() { return board; } // получение доступа к модели доски
  void updateBoard(); // обновление отображения всей доски
  void setBoard(
      const SudokuBoard &newBoard); // устанавливает новое игровое поле (доску)

private:
  // Сетка для размещения ячеек
  QGridLayout *gridLayout;
  SudokuCell *cells[9][9];
  SudokuBoard board;
  QLabel *timerLabel; // Метка для таймера
  QLabel *errorLabel; // Метка для ошибок

signals:
  // Сигнал при клике на ячейку
  void CellClicked(int row, int col);
  // Сигнал при вводе числа в ячейку
  void numberEntered(int row, int col, int value);
};
