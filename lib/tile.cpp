#include "tile.h"

namespace MyAscii {

    Tile::Tile(unsigned int id) {
        asciiChar = rand() % ASCII_TABLE_RANGE + 1 + ASCII_TABLE_START;
        this->id = id;
    }

    char Tile::getHiddenChar(void) {
        return hiddenChar;
    }

    char Tile::getAsciiChar(void) {
        return asciiChar;
    }

    unsigned int Tile::getId(void) {
        return id;
    }

}