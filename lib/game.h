#pragma once

#include "score.h"
#include "player.h"
#include "playfield.h"
#include <iostream>
#include <windows.h>
#include "console.h"
#include <stdio.h>
#include <algorithm>

namespace MyAscii {

    class Game {
        public:
            Game(Player * player, Console * console);
        
        public:
            Score start(unsigned int difficulty);
            void setDifficulty(unsigned int difficulty);

        private:
            Player * player;
            Console * console;
            unsigned int difficulty = 0;
            unsigned int fieldEdgeSize = gameParameters[difficulty][0];
            unsigned int pairSize = gameParameters[difficulty][1];
            std::vector<int> correctAnswers;

            // {fieldEdgeSize, pairSize}
            unsigned int gameParameters[5][2] = {
                {2, 2},
                {4, 2},
                {6, 2},
                {8, 2},
                {9, 3}
            };
    };
};