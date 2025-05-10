#include "DifficultyDialog.h"

DifficultyDialog::DifficultyDialog(QWidget *parent) : QDialog(parent), selectedDifficulty_(1) {
    setWindowTitle("Выбор сложности");
    resize(250, 150);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("Выберите уровень сложности:", this);
    layout->addWidget(label);

    QPushButton *easyButton = new QPushButton("Легкий", this);
    QPushButton *mediumButton = new QPushButton("Средний", this);
    QPushButton *hardButton = new QPushButton("Сложный", this);

    layout->addWidget(easyButton);
    layout->addWidget(mediumButton);
    layout->addWidget(hardButton);

    connect(easyButton, &QPushButton::clicked, this, &DifficultyDialog::onEasyClicked);
    connect(mediumButton, &QPushButton::clicked, this, &DifficultyDialog::onMediumClicked);
    connect(hardButton, &QPushButton::clicked, this, &DifficultyDialog::onHardClicked);
}

void DifficultyDialog::onEasyClicked() {
    selectedDifficulty_ = 0;  // Легкий
    accept();
}

void DifficultyDialog::onMediumClicked() {
    selectedDifficulty_ = 1;  // Средний
    accept();
}

void DifficultyDialog::onHardClicked() {
    selectedDifficulty_ = 2;  // Сложный
    accept();
}
