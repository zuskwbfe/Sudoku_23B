#pragma once
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class DifficultyDialog : public QDialog {
    Q_OBJECT

public:
    explicit DifficultyDialog(QWidget *parent = nullptr);

    int selectedDifficulty() const { return selectedDifficulty_; }

private slots:
    void onEasyClicked();
    void onMediumClicked();
    void onHardClicked();

private:
    int selectedDifficulty_;
};
