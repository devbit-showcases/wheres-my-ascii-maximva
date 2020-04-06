#include "playfield.h"

namespace MyAscii {

    PlayField::PlayField(unsigned int fieldEdge) {
        fieldSize = fieldEdge * fieldEdge;
        generatePlayField();
    }

    std::vector<Tile> PlayField::getPlayField(void) {
        return playField;
    }

    void PlayField::generatePlayField(void) {
        time_t seed = time(NULL);
        for (unsigned int c = 0; c < fieldSize; c++) {
            Tile tile;
            playField.push_back(tile);
        }
    }
}