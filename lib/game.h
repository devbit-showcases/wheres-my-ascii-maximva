#pragma once

#include "score.h"
#include "player.h"
#include "playfield.h"
#include <iostream>
#include <windows.h>
#include "console.h"
#include <stdio.h>
#include <algorithm>
#include "scorecard.h"
#include <time.h>

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
            unsigned int pair_size = gameParameters[difficulty][1];
            std::vector<int> correctAnswers;

            // {fieldEdgeSize, pairSize}
            unsigned int gameParameters[5][2] = {
                {4, 2},
                {6, 3},
                {6, 3}, // Same as before but with possible double characters  AND TIMER
                {9, 3}  // Insane, large, tripple tile and possible double characters  AND TIMER
            };
    };
};