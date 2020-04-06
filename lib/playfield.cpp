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
        std::vector<Tile> tiles;
        std::vector<char> used_characters;

        // Fill the playfield with tiles
        for (unsigned int c = 0; c < fieldSize / pair_size; c++) {
            Tile tile(c);
            for (unsigned int d = 0; d < pair_size; d++) {
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