#include "view/MainWindow.h"
#include "view/SudokuCell.h"
#include <iostream>

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow window;
  window.UpdateCell(0, 0, 5);
  window.UpdateCell(4, 2, 3);
  window.UpdateCell(8, 8, 9);
  window.show();
  return app.exec();
}
