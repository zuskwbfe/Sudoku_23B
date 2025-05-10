#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class StartWindow : public QWidget {
    Q_OBJECT
public:
    explicit StartWindow(QWidget *parent = nullptr);

signals:
    void startGame(); // Сигнал при нажатии кнопки "Новая игра"
};
