#pragma once

#include <iostream>
#include "tile.h"
#include <vector>
#include <time.h>
#include <iterator>

namespace MyAscii {

    class PlayField {
        public:
            PlayField(unsigned int fieldEdgeSize, unsigned int pairSize, unsigned int difficulty);
            
        public:
            std::vector<Tile> getPlayField(void);
            unsigned int getFieldEdgeSize(void);

        private:
            void generatePlayField(void);
        
        private:
            unsigned int fieldEdgeSize = 2;
            unsigned int fieldSize = 4;
            unsigned int pairSize = 2;
            unsigned int difficulty = 0;
            std::vector<Tile> playField;
            unsigned int charCount[26] = {0};
            std::vector<int> possibleCharAttributes = {
                        0x13, 0x1B, 0x20, 0x2F, 0x31, 0x3B, 0x4F, 0x4A,
                        0x5C, 0x5F, 0x60, 0x6F, 0x70, 0x78, 0x7F, 0x80,
                        0x87, 0x8F, 0x9F, 0x9B, 0xA0, 0xAC, 0xB3, 0xB5,
                        0xC7, 0xCB, 0xDF, 0xDB, 0xD1, 0xCF, 0xB9, 0xF8
                        };

    };
};