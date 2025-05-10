#pragma once
#include "MainWindow.h"
#include "StartWindow.h"
#include "SudokuBoard.h"
#include "SudokuGenerator.h"
#include "SudokuSolver.h"
#include <QMessageBox> // для отображения всплывающих сообщений пользователю
#include <QTimer>      // Добавляем для таймера

// контроллер для управления игровой логикой судоку
class SudokuController : public QObject {
  Q_OBJECT

public:
  SudokuController(SudokuBoard *board, MainWindow *view,
                   QObject *parent = nullptr);
  // начать новую игру с указанной сложностью
  void newGame(int difficulty);

  // Методы для таймера и ошибок
  void startTimer();
  void stopTimer();
  int getErrorCount() const { return errorCount_; }
  QString formatTime(int seconds) const;

private slots:
  // обработчик клика по клетке
  void onCellClicked(int row, int col);
  // обработчик ввода числа
  void onNumberEntered(int row, int col, int value);
  // проверка допустимости хода
  bool checkInput(int row, int col, int value);
  // проверка завершения игры
  void checkSolved();
  // обновления таймера
  void updateTimer();

private:
  SudokuBoard *board_;
  MainWindow *view_;
  BacktrackingSolver solver_; // Use a concrete solver
  SudokuGenerator generator_; // And a generator
  int selectedRow_;
  int selectedCol_;
  bool gameStarted_;

  // Добавляем таймер и счётчики
  QTimer *timer_;
  int secondsElapsed_; // таймер
  int errorCount_;     // счетчик ошибок
};
