#pragma once

#include "score.h"
#include "player.h"
#include "playfield.h"
#include <iostream>

namespace MyAscii {

    class Game {
        public:
            Game(Player * player, unsigned int difficulty);
        
        public:
            Score start(void);
        
        // private:
        //     void setGameParameters(void);

        private:
            Player * player;

            // {fieldEdgeSize, pairSize}
            unsigned int gameParameters[5][2] = {
                {2, 2},
                {4, 2},
                {6, 2},
                {8, 2},
                {9, 3}
            };

            unsigned int difficulty = 0;

    };

};