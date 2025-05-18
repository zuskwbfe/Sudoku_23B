#include "controller/SudokuController.h"
#include "model/SudokuBoard.h"
#include "model/SudokuGenerator.h"
#include "model/SudokuSolver.h"
#include "view/DifficultyDialog.h"
#include "view/MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // Создаём модель, представление и контроллер
  SudokuBoard board;
  MainWindow mainWindow;
  SudokuController controller(&board, &mainWindow);

  // Настраиваем связи
  mainWindow.setController(&controller);

  // Показываем главное окно (в нём уже есть меню)
  mainWindow.show();

  return app.exec();

  // // Создаём стартовое окно
  // StartWindow *startWindow = new StartWindow();
  // startWindow->show();

  // // Подключаем сигнал выбора сложности
  // QObject::connect(startWindow, &StartWindow::gameSelected,
  //              controller, &SudokuController::newGame);

  // startWindow->show();

  // QObject::connect(controller, &SudokuController::gameFinished, [=]() {
  // mainWindow->hide();     // скрыть главное окно
  // startWindow->show();    // снова показать стартовое окно
  // });
  // return app.exec();
}
