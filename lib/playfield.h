#pragma once

#include <iostream>
#include "tile.h"
#include <vector>
#include <time.h>
#include <iterator>

namespace MyAscii {

    class PlayField {
        public:
            PlayField(void);
            PlayField(unsigned int fieldEdgeSize, unsigned int pairSize, unsigned int difficulty, bool secret);
            
        public:
            std::vector<Tile> get_playfield(void);
            unsigned int get_playfield_edgesize(void);

        private:
            void generate_playfield(void);
            void randomize_playfield(void);
            void create_tile_set(Tile * tile);
            void add_tile_colors(Tile * tile);
            void add_unique_character(Tile *tile, int charAlphabetPosition);
        
        private:
            const int GREEN_BACK_WHITE_FRONT = 3;
            unsigned int playfieldEdgesize = 2;
            unsigned int playfieldSize = 4;
            unsigned int setSize = 2;
            unsigned int difficulty = 0;

            std::vector<Tile> playField;
            std::vector<Tile> nonRandomTiles;
            unsigned int charCount[26] = {0};
            std::vector<int> charAttributes = {
                        0x13, 0x1B, 0x29, 0x2F, 0x31, 0x3B, 0x4F, 0x4A,
                        0x5C, 0x5F, 0x60, 0x6F, 0x70, 0x78, 0x7F, 0x80,
                        0x87, 0x8F, 0x9F, 0x9B, 0xA0, 0xAC, 0xB3, 0xB5,
                        0xC7, 0xCB, 0xDF, 0xDB, 0xD1, 0xCF, 0xB9, 0xF8
                        };
            bool secret = false;
            bool monochromeTiles = false;
            bool preventDuplicateChars = true;
            

    };
};