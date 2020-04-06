#include "tile.h"

namespace MyAscii {

    Tile::Tile(void) {
        srand (time(NULL));
        asciiChar = rand() % ASCII_TABLE_RANGE + 1 + ASCII_TABLE_START;

    }

    char Tile::getHiddenChar(void) {
        return hiddenChar;
    }

    char Tile::getAsciiChar(void) {
        return asciiChar;
    }



}