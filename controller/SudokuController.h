#pragma once
#include "MainWindow.h"
#include "SudokuBoard.h"
#include "SudokuGenerator.h"
#include "SudokuSolver.h"
#include <QMessageBox> // для отображения всплывающих сообщений пользователю

// контроллер для управления игровой логикой судоку
class SudokuController : public QObject {
  Q_OBJECT

public:
  SudokuController(SudokuBoard *board, MainWindow *view,
                   QObject *parent = nullptr);
  // начать новую игру с указанной сложностью
  void newGame(int difficulty);

private slots:
  // обработчик клика по клетке
  void onCellClicked(int row, int col);
  // обработчик ввода числа
  void onNumberEntered(int row, int col, int value);
  // проверка допустимости хода
  bool checkInput(int row, int col, int value);
  // проверка завершения игры
  void checkSolved();

private:
  SudokuBoard *board_;
  MainWindow *view_;
  BacktrackingSolver solver_; // Use a concrete solver
  SudokuGenerator generator_; // And a generator
  int selectedRow_;
  int selectedCol_;
  bool gameStarted_;
};
