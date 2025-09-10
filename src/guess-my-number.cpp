#include <clocale>
#include <iostream>
#include <vector>
#include <fstream>
#include "parameters.h"
#include "game/exception.h"
#include "game/scores.h"
#include "game/levels.h"
#include "game/help.h"

Game::Scores scores = Game::Scores();;

void run(int max) {
    srand(time(nullptr));

    std::string username = "";

    std::cout << "Привет, как тебя зовут?" << std::endl;
    std::cin >> username;

    if (!username.length() || username == " ") {
        return;
    }

    std::cout << "Отгадай число, которое я загадал!" << std::endl;

    const int min = 1;
    const int randomValue = (rand() % (max - min + 1)) + min;
    int guess = 0;
    int attempts = 0;

    while (guess != randomValue) {
        std::cin >> guess;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();

            std::cout << "Некорректное значение значение, попробуй еще раз" << std::endl;

            continue;
        }

        attempts++;

        if (guess > randomValue) {
            std::cout << "Моё число меньше " << guess << std::endl;
        }
        else if (guess < randomValue) {
            std::cout << "Моё число больше " << guess << std::endl;
        }
    }

    std::cout << "Поздравляю, ты отгадал с " << attempts << " попытки" << std::endl;

    scores.update(username, attempts);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    std::map<std::string, std::string> parametersMap = parseParameters(argc, argv);

    if (!parametersMap.size()) {
        Game::Help::show();

        return 1;
    }

    try {
        if (parametersMap.count("table")) {
            scores.show();

            return 0;
        }

        int max = Game::Levels::getMaxNumberForLevel(Game::Levels::EASY);
        std::string level = parametersMap["level"];
        std::string parameterMax = parametersMap["max"];

        if (!(level.empty() || parameterMax.empty())) {
            throw Game::Exception("Запрещено одновременное использование параметров -level и -max");
        }

        if (!level.empty()) {
            max = Game::Levels::getMaxNumberForLevel(stoi(level));

            if (!max) {
                throw Game::Exception("Параметр level должен быть от 1 до 3");
            }
        }

        if (!parameterMax.empty()) {
            const int max_ = stoi(parameterMax);

            if (max_ <= 1)  {
                throw Game::Exception("Параметр max должен быть больше 1");
            }

            max = max_;
        }

        run(max);
    } catch (Game::Exception& e) {
        std::cout << e.what() << std::endl;

        return 1;
    }

    return 0;
}