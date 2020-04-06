#include <iostream>
#include "../lib/tile.h"
#include "../lib/playfield.h"

using namespace MyAscii;

int main(void) {    
    
    PlayField playfield(4);
    std::vector<Tile> playField = playfield.getPlayField();

    for (unsigned int c = 0; c < playField.size(); c++) {
        std::cout << playField[c].getAsciiChar() << " ";
    }
    std::cout << std::endl;


    return 0;
}