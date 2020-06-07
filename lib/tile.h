#pragma once

#include <iostream>

namespace MyAscii {
    class Tile {
        public:
            Tile(unsigned int id, int flipped_attribute, bool secret);
        
        public:
            char get_covered_char(void);
            char get_flipped_char(void);
            int get_flipped_attribute(void);
            int get_covered_attribute(void);
            unsigned int get_id(void);
            bool is_flipped(void);
            void set_random_char(void);
            void flip_tile(void);

        private:
            unsigned int id = 0;
            bool turned = false;
            bool secret = false;
            char flippedChar;
            wchar_t coveredChar = L'?';
            wchar_t secretHiddenChar = L'$';
            int flippedAttribute = 0x31;
            int coveredAttribute = 0x20;
            unsigned int ASCII_TABLE_START = 'A';
            unsigned int ASCII_TABLE_END = 'Z';
            unsigned int ASCII_TABLE_RANGE = ASCII_TABLE_END - ASCII_TABLE_START;
    };
};