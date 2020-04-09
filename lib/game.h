#pragma once

#include "score.h"
#include "player.h"
#include "playfield.h"
#include <iostream>
#include <windows.h>
#include "console.h"
#include <stdio.h>

namespace MyAscii {

    class Game {
        public:
            Game(Player * player, unsigned int difficulty);
        
        public:
            Score start(void);

        private:
            Player * player;
            unsigned int difficulty = 0;

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