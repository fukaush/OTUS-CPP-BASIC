#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "exception.h"
#include "scores.h"

const char* filename = "high_scores.txt";
const int countScores = 3;

Game::Scores::Scores() {
    std::fstream file;

    try {
        file.open(filename);

        std::string line;

        while (std::getline(file, line)) {
            int commaPos = line.find(' ');
            std::string name = line.substr(0, commaPos);
            int value = stoi(line.substr(commaPos + 1));

            scoreMap[name] = value;
        }
    } catch(...) {
        //
    } 

    if (file.is_open()) {
        file.close();
    }
}

void Game::Scores::show() {
    if (!scoreMap.size()) {
        std::cout << "| Результатов нет :(" << std::endl;

        return;
    }

    std::vector<std::pair<std::string, int>> vec(scoreMap.begin(), scoreMap.end());

    std::sort(vec.begin(), vec.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return a.second < b.second;
    });

    std::cout << "| Топ " << countScores << " лучших результатов:" << std::endl;
    std::cout << "| " << std::endl;

    int count = std::min(countScores, static_cast<int>(scoreMap.size()));

    for (int i = 0; i < count; i++) {
        std::cout << "| " << vec[i].first << " " << vec[i].second << std::endl;
    }
}

void Game::Scores::update(std::string name, int score) {
    std::fstream file;

    try {
        file.open(filename, std::ofstream::out);

        scoreMap[name] = score;

        for (auto const& [key, val] : scoreMap) {
            file << key << " " << val << std::endl;
        }
    } catch(...) {
        //
    }

    if (file.is_open()) {
        file.close();
    }
}