#pragma once
#include <QDialog>
#include <QTableWidget>
#include "../model/GameRecords.h"

class RecordsWindow : public QDialog {
    Q_OBJECT
public:
    explicit RecordsWindow(QWidget* parent = nullptr);
    void showRecords(const GameRecords& records);

private:
    QTableWidget* table;
    void setupTable();
};
