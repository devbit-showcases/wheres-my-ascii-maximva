#pragma once

#include <iostream>
#include "tile.h"
#include <vector>
#include <time.h>

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
            time_t seed = time(NULL);

    };
};