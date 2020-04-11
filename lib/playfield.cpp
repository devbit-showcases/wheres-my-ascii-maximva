#include "playfield.h"

namespace MyAscii {

    PlayField::PlayField(unsigned int fieldEdgeSize, unsigned int pairSize, unsigned int difficulty) {
        this->fieldEdgeSize = fieldEdgeSize;
        this->pairSize = pairSize;
        this->difficulty = difficulty;
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

        // Fill the playfield with tiles
        for (unsigned int c = 0; c < fieldSize / pairSize; c++) {
            int attributeNumber = rand() % possibleCharAttributes.size();
            
            // deleting the used attribute values from the vector doesn't seem to work
            // possibleCharAttributes.erase(std::begin(possibleCharAttributes) + attributeNumber);

            Tile tile(c, possibleCharAttributes[attributeNumber]);
            int charAlphabetPosition = tile.getAsciiChar() - 'A';
            charCount[charAlphabetPosition]++;

            // Make sure there's no double characters below sertain level
            while (difficulty < 3 && charCount[charAlphabetPosition] > 1) {
                charCount[charAlphabetPosition]--;
                tile.setRandomChar();
                charAlphabetPosition = tile.getAsciiChar() - 'A';
                charCount[charAlphabetPosition]++;
            }

            // Add pairsize amount of tiles to the tiles vector
            for (unsigned int d = 0; d < pairSize; d++) {
                tiles.push_back(tile);
            }
        }

        // Randomize the playfield tile positions
        for (unsigned int c = 0; c < fieldSize; c++) {
            unsigned int place = rand() % tiles.size();
            playField.push_back( tiles[place] );
            tiles.erase(std::begin(tiles) + place);
        }
    }
    
}