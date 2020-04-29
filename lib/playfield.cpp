#include "playfield.h"

namespace MyAscii {

    /**
     * Creates an instance of PlayField with default values
     */
    PlayField::PlayField(void)
    :PlayField(4, 2, 0, false) {

    }


    /**
     * Creates an instance of PlayField
     */
    PlayField::PlayField(unsigned int playfieldEdgesize, unsigned int setSize, unsigned int difficulty, bool secret) {
        this->playfieldEdgesize = playfieldEdgesize;
        this->setSize = setSize;
        this->difficulty = difficulty;
        this->secret = secret;
        playfieldSize = playfieldEdgesize * playfieldEdgesize;
        generate_playfield();
    }


    /**
     * Returns the playfield vector with all the tiles
     */
    std::vector<Tile> PlayField::get_playfield(void) {
        return playField;
    }


    /**
     * Returns the size of an edge of the playfield
     */
    unsigned int PlayField::get_playfield_edgesize(void) {
        return playfieldEdgesize;
    }


    /**
     * Generates the tiles that occupy the playfield
     */
    void PlayField::generate_playfield(void) {
        const bool MONOCHROME_TILES = (difficulty == 2 || difficulty == 3);
        const int GREEN_BACK_WHITE_FRONT = 3;
        int attributeNumber = GREEN_BACK_WHITE_FRONT;

        for (unsigned int id = 0; id < playfieldSize / setSize; id++) {
            const bool PREVENT_DUPLICATE_CHARS = (difficulty < 3);
            Tile tile(id, possibleCharAttributes[attributeNumber], secret);
            int charAlphabetPosition = tile.get_flipped_char() - 'A';
            charCount[charAlphabetPosition]++;

            if (!MONOCHROME_TILES) {
                attributeNumber = rand() % possibleCharAttributes.size();
                possibleCharAttributes.erase(std::begin(possibleCharAttributes) + attributeNumber); // Erase attribute from vector
            }
            
            while (PREVENT_DUPLICATE_CHARS && charCount[charAlphabetPosition] > 1) {
                charCount[charAlphabetPosition]--;
                tile.set_random_char();
                charAlphabetPosition = tile.get_flipped_char() - 'A';
                charCount[charAlphabetPosition]++;
            }
            add_tiles_set(&tile);
        }
        randomize_playfield();
    }


    /**
     * Adds a set (size of a pair) to the (non-random) tile vector
     */
    void PlayField::add_tiles_set(Tile * tile) {
        for (unsigned int d = 0; d < setSize; d++) {
                nonRandomTiles.push_back((*tile));
        }
    }


    /**
     * Randomizes the position of the tiles
     */
    void PlayField::randomize_playfield(void) {
        for (unsigned int c = 0; c < playfieldSize; c++) {
            unsigned int place = rand() % nonRandomTiles.size();
            playField.push_back( nonRandomTiles[place] );
            nonRandomTiles.erase(std::begin(nonRandomTiles) + place);
        }
    }
    
}