#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "DifficultyDialog.h"

class StartWindow : public QWidget {
    Q_OBJECT
public:
    explicit StartWindow(QWidget *parent = nullptr);

signals:
    void gameSelected(int difficulty);  // Сигнал для передачи уровня сложности

private:
    void startGame(); // Сигнал при нажатии кнопки "Новая игра"
};
