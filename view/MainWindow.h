#pragma once
#include "../controller/SudokuController.h"
#include "../model/SudokuBoard.h"
#include "DifficultyDialog.h"
#include "SudokuCell.h"
#include "RecordsWindow.h"
#include <QGridLayout>
#include <QLabel> // Для отображения таймера и ошибок
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>

class SudokuController;

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
  // SudokuBoard &getBoard() { return board; } // получение доступа к модели
  // доски
  void updateBoard(); // обновление отображения всей доски
  void setController(SudokuController *controller);
  void refreshHighlight();

public slots:
  void showMainMenu();
  void handleGameCompleted();
  void handleCellSelected(int row, int col);
  void handleCellDoubleClicked(int row, int col);

private slots:
  void handleNewGame();
  void showRecords(); // Новый слот для рекордов

private:
  void createGameScreen();
  void createMainMenu();

  // Структура интерфейса
  QStackedWidget *stackedWidget; // Основной контейнер для переключения экранов
  QWidget *menuScreen;
  QWidget *gameScreen;

  // Сетка для размещения ячеек
  QGridLayout *gridLayout;
  SudokuCell *cells[9][9];
  // SudokuBoard board;
  QLabel *timerLabel; // Метка для таймера
  QLabel *errorLabel; // Метка для ошибок

  // Навигация
  DifficultyDialog *difficultyDialog;
  SudokuController *controller;

  // Методы для управления подсветкой
  void clearHighlights();
  void highlightRelatedCells(int row, int col);
  void highlightSameDigits(int row, int col);
  void keyPressEvent(QKeyEvent *event) override;

signals:
  // Сигнал при клике на ячейку
  void CellClicked(int row, int col);
  void CellDoubleClicked(int row, int col);
  // Сигнал при вводе числа в ячейку
  void numberEntered(int row, int col, int value);
};
