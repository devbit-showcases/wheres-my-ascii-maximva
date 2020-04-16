#pragma once

#include <iostream>
#include <time.h>

namespace MyAscii {
    class Tile {
        public:
            Tile(unsigned int id, int flipped_attribute);
        
        public:
            char getHiddenChar(void);
            char getAsciiChar(void);
            unsigned int getId(void);
            void turnCard(void);
            bool isTurned(void);
            void setRandomChar(void);
            int getCharFlippedAttribute(void);
            int getCharCoveredAttribute(void);
            void setHiddenChar(wchar_t hidden_char);

        private:
            unsigned int id = 0;
            bool turned = false;
            char flipped_char;
            wchar_t hidden_char = L'?';
            int flipped_attribute = 0x31;
            int covered_attribute = 0x20;
            unsigned int ASCII_TABLE_START = 'A';
            unsigned int ASCII_TABLE_END = 'Z';
            unsigned int ASCII_TABLE_RANGE = ASCII_TABLE_END - ASCII_TABLE_START;
    };
};