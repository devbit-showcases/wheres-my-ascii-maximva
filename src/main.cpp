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
        std::cout << playField[c].getAsciiChar() << "(" << playField[c].getId() << ")" << " ";
        if ((c + 1) % 4 == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;


    return 0;
}