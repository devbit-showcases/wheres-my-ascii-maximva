#include "playfield.h"

namespace MyAscii {

    PlayField::PlayField(unsigned int fieldEdgeSize, unsigned int pairSize) {
        this->fieldEdgeSize = fieldEdgeSize;
        this->pairSize = pairSize;
        fieldSize = fieldEdgeSize * fieldEdgeSize;
        generatePlayField();
    }

    std::vector<Tile> PlayField::getPlayField(void) {
        return playField;
    }

    unsigned int PlayField::getFieldEdgeSize(void) {
        return fieldEdgeSize;
    }

    void PlayField::generatePlayField(void) {
        std::vector<Tile> tiles;
        std::vector<char> used_characters;

        // Fill the playfield with tiles
        for (unsigned int c = 0; c < fieldSize / pairSize; c++) {
            Tile tile(c);
            for (unsigned int d = 0; d < pairSize; d++) {
                tiles.push_back(tile);
            }
            used_characters.push_back(tile.getAsciiChar()); // Store used chars to later check for doubles
        }

        // Randomize the playfield positions
        for (unsigned int c = 0; c < fieldSize; c++) {
            unsigned int place = rand() % tiles.size();
            playField.push_back( tiles[place] );
            tiles.erase(std::begin(tiles) + (place));
        }
    }

}