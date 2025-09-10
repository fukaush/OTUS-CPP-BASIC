#pragma once

namespace Game::Levels {
    const int EASY = 1;
    const int NORMAL = 2;
    const int HARD = 3;
    /* Получить максимальное число для уровня */
    int getMaxNumberForLevel(int level);
}