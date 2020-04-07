#include "game.h"

namespace MyAscii {

    Game::Game(Player * player, unsigned int difficulty) {
        this->player = player;
        this->difficulty = difficulty;
    }

    Score Game::start(void) {
        Score score;
        unsigned int fieldEdgdeSize = gameParameters[difficulty][0];
        unsigned int pairSize = gameParameters[difficulty][1];

        PlayField playfield(fieldEdgdeSize, pairSize);
        std::vector<Tile> tiles = playfield.getPlayField();

        for (unsigned int c = 0; c < tiles.size(); c++) {
            std::cout << tiles[c].getAsciiChar() << "(" << tiles[c].getId() << ")" << " ";
            if ((c + 1) % playfield.getFieldEdgeSize() == 0) {
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
        
        return score;
    }

}