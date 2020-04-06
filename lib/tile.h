#pragma once

#include <iostream>
#include <time.h>

namespace MyAscii {

    class Tile {
        public:
            Tile(unsigned int id);
            char getHiddenChar(void);
            char getAsciiChar(void);
            unsigned int getId(void);

        private:
            unsigned int id = 0;
            char asciiChar;
            char hiddenChar = '?';
            unsigned int ASCII_TABLE_START = 'A';
            unsigned int ASCII_TABLE_END = 'Z';
            unsigned int ASCII_TABLE_RANGE = ASCII_TABLE_END - ASCII_TABLE_START;
    };
};