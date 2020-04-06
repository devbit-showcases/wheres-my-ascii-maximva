#include <iostream>
#include "../lib/tile.h"
#include "../lib/playfield.h"
#include "time.h"

using namespace MyAscii;

int main(void) {
    
    srand(time(NULL));

    PlayField playfield(4);
    std::vector<Tile> playField = playfield.getPlayField();

    for (unsigned int c = 0; c < playField.size(); c++) {
        std::cout << playField[c].getAsciiChar() << " ";
    }
    std::cout << std::endl;


    PlayField playfield2(4);
    std::vector<Tile> playField2 = playfield2.getPlayField();

    for (unsigned int c = 0; c < playField2.size(); c++) {
        std::cout << playField2[c].getAsciiChar() << " ";
    }
    std::cout << std::endl;


    return 0;
}