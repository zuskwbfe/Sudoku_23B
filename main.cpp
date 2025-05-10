#include "controller/SudokuController.h"
#include "model/SudokuBoard.h"
#include "model/SudokuGenerator.h"
#include "model/SudokuSolver.h"
#include "view/MainWindow.h"
#include "view/StartWindow.h"
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

    // Когда нажата "Новая игра"
    QObject::connect(startWindow, &StartWindow::startGame, [&]() {
        controller->newGame(1); // средняя сложность
        mainWindow->setWindowTitle("Судоку");
        mainWindow->resize(450, 450);
        mainWindow->show();
        startWindow->close();
    });

    return app.exec();
}
