#pragma once

#include <iostream>
#include "tile.h"
#include <vector>

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
            // Tile f[fieldSize * fieldSize];
    };
};