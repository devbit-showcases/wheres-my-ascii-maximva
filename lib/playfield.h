#pragma once

#include <iostream>
#include "tile.h"
#include <vector>
#include <time.h>
#include <iterator>

namespace MyAscii {

    class PlayField {
        public:
            PlayField(unsigned int fieldSize);
            
        public:
            std::vector<Tile> getPlayField(void);

        private:
            void generatePlayField(void);
        
        private:
            unsigned int fieldSize;
            unsigned int pair_size = 2;
            std::vector<Tile> playField;

    };
};