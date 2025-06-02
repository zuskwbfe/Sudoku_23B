#include "RecordsWindow.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QTableWidgetItem>

RecordsWindow::RecordsWindow(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Лучшие результаты");
    resize(400, 300);

    table = new QTableWidget(this);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Игрок", "Время", "Сложность"});
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(table);
    setupTable();
}

void RecordsWindow::setupTable() {
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table->setColumnWidth(1, 100);
    table->setColumnWidth(2, 100);
}

void RecordsWindow::showRecords(const GameRecords& records) {
    table->setRowCount(0);

    auto addRecords = [&](const std::vector<GameRecords::Record>& recs, const QString& level) {
        if (recs.empty()) return;
        for (const auto& record : recs) {
            const int row = table->rowCount();
            table->insertRow(row);
            table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(record.playerName)));
            table->setItem(row, 1, new QTableWidgetItem(QString("%1 сек").arg(record.timeSeconds)));
            table->setItem(row, 2, new QTableWidgetItem(level));
        }
    };

    addRecords(records.getRecords(Difficulty::EASY), "Легкая");
    addRecords(records.getRecords(Difficulty::MEDIUM), "Средняя");
    addRecords(records.getRecords(Difficulty::HARD), "Сложная");
}
