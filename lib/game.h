#pragma once

#include "score.h"
#include "player.h"

namespace MyAscii {

    class Game {
        public:
            Game(Player * player);
        
        public:
            Score start(void);

        private:
            Player * player;

    };

};