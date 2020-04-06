#pragma once

#include <iostream>
#include <time.h>
// #include <stdlib.h>

namespace MyAscii {

    class Tile {
        public:
            Tile(void);
            char getHiddenChar(void);
            char getAsciiChar(void);

        private:
            char asciiChar;
            char hiddenChar = '?';
            const unsigned int ASCII_TABLE_START = 65; // A in ASCII table
            const unsigned int ASCII_TABLE_END = 90; // Z in ASCII table
            const unsigned int ASCII_TABLE_RANGE = ASCII_TABLE_END - ASCII_TABLE_START;
    };
};