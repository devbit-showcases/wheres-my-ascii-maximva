#include "tile.h"

namespace MyAscii {

    Tile::Tile(void) {

        asciiChar = rand() % 26 + 65;

    }

    char Tile::showHiddenChar(void) {
        return hiddenChar;
    }



}