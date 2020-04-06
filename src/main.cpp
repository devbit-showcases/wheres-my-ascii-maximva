#include <iostream>
#include "../lib/tile.h"
#include "../lib/playfield.h"

using namespace MyAscii;

int main(void) {

    Tile tile;

    std::cout << "Random ascii char: " << tile.getAsciiChar() << std::endl;
    std::cout << "Random ascii char: " << tile.getAsciiChar() << std::endl;
    
    
    PlayField playfield(3);
    std::vector<Tile> playField = playfield.getPlayField();

    for (unsigned int c = 0; c < playField.size(); c++) {
        std::cout << playField[c].getAsciiChar() << " ";
    }
    std::cout << std::endl;


    return 0;
}