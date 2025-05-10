#include "StartWindow.h"

StartWindow::StartWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Добро пожаловать в Судоку");
    resize(300, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QPushButton *startButton = new QPushButton("Новая игра", this);

    layout->addStretch();
    layout->addWidget(startButton);
    layout->addStretch();

    connect(startButton, &QPushButton::clicked, this, &StartWindow::startGame);
}

void StartWindow::startGame() {
    // Открываем диалог выбора сложности
    DifficultyDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        emit gameSelected(dialog.selectedDifficulty());  // Отправляем выбранный уровень сложности
    }
}
