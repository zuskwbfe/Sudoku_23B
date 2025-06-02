#include "GameRecords.h"
#include <iostream>

GameRecords::GameRecords() {
    loadFromFile();
}

void GameRecords::loadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return;

    Record record;
    while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        switch (record.level) {
            case Difficulty::EASY: easyRecords.push_back(record); break;
            case Difficulty::MEDIUM: mediumRecords.push_back(record); break;
            case Difficulty::HARD: hardRecords.push_back(record); break;
        }
    }
}

void GameRecords::saveToFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    if (!file) {
        std::cerr << "Failed to open records file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& r : easyRecords)
        file.write(reinterpret_cast<const char*>(&r), sizeof(Record));
    for (const auto& r : mediumRecords)
        file.write(reinterpret_cast<const char*>(&r), sizeof(Record));
    for (const auto& r : hardRecords)
        file.write(reinterpret_cast<const char*>(&r), sizeof(Record));
}

void GameRecords::addRecord(const Record& newRecord) {
    auto& records = [&]() -> std::vector<Record>& {
        switch (newRecord.level) {
            case Difficulty::EASY: return easyRecords;
            case Difficulty::MEDIUM: return mediumRecords;
            case Difficulty::HARD: return hardRecords;
            default: return easyRecords;
        }
    }();

    records.push_back(newRecord);
    // Сортируем по времени (от меньшего к большему)
    std::sort(records.begin(), records.end(),
        [](const Record& a, const Record& b) { return a.timeSeconds < b.timeSeconds; });

    // Оставляем только топ-10
    if (records.size() > 10)
        records.resize(10);

    saveToFile();
}

const std::vector<GameRecords::Record>& GameRecords::getRecords(Difficulty difficulty) const {
    switch (difficulty) {
        case Difficulty::EASY: return easyRecords;
        case Difficulty::MEDIUM: return mediumRecords;
        case Difficulty::HARD: return hardRecords;
        default: return easyRecords;
    }
}
