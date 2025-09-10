#include "levels.h"

int Game::Levels::getMaxNumberForLevel(int level) {
    switch (level) {
        case Game::Levels::EASY:
            return 10;
        case Game::Levels::NORMAL:
            return 50;
        case Game::Levels::HARD:
            return 100;
    }

    return 0;
}