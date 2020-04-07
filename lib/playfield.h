#pragma once

#include <iostream>
#include "tile.h"
#include <vector>
#include <time.h>
#include <iterator>

namespace MyAscii {

    class PlayField {
        public:
            PlayField(unsigned int fieldEdgeSize, unsigned int pairSize);
            
        public:
            std::vector<Tile> getPlayField(void);
            unsigned int getFieldEdgeSize(void);

        private:
            void generatePlayField(void);
        
        private:
            unsigned int fieldEdgeSize = 2;
            unsigned int fieldSize = 4;
            unsigned int pairSize = 2;
            std::vector<Tile> playField;

    };
};