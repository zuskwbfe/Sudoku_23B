#include "controller/SudokuController.h"
#include "model/SudokuBoard.h"
#include "model/SudokuGenerator.h"
#include "model/SudokuSolver.h"
#include "view/MainWindow.h"
#include "view/StartWindow.h"
#include "view/DifficultyDialog.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Создаём модель, представление и контроллер
    SudokuBoard board;
    MainWindow *mainWindow = new MainWindow();
    SudokuController *controller = new SudokuController(&board, mainWindow);

    // Создаём стартовое окно
    StartWindow *startWindow = new StartWindow();
    startWindow->show();

    // Подключаем сигнал выбора сложности
QObject::connect(startWindow, &StartWindow::gameSelected,
                 controller, &SudokuController::newGame);

    startWindow->show();
    return app.exec();
}
