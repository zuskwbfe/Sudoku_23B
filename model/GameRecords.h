#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>

enum class Difficulty {
    EASY,
    MEDIUM,
    HARD
};

class GameRecords {
public:
    struct Record {
        std::string playerName;
        int timeSeconds;
        Difficulty level;
    };

    GameRecords();
    void loadFromFile(const std::string& filename = "records.dat");
    void saveToFile(const std::string& filename = "records.dat");
    void addRecord(const Record& newRecord);
    const std::vector<Record>& getRecords(Difficulty difficulty) const;

private:
    std::vector<Record> easyRecords;
    std::vector<Record> mediumRecords;
    std::vector<Record> hardRecords;
};
