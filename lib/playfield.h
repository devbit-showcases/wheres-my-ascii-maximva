#pragma once

#include <iostream>
#include "tile.h"
#include <vector>
#include <time.h>
#include <algorithm>
#include <random>

namespace MyAscii {

    class PlayField {
        public:
            PlayField(unsigned int fieldSize);
            std::vector<Tile> getPlayField(void);

        private:
            void generatePlayField(void);
        
        private:
            unsigned int fieldSize;
            std::vector<Tile> playField;
            unsigned int pair_size = 2;

    };
};