#pragma once
#include <string>
#include <unordered_map>

namespace Game {
    class Scores {
        std::unordered_map<std::string, int> scoreMap;
    public:
        Scores();
        void show();
        void update(std::string name, int score);
    };
}
