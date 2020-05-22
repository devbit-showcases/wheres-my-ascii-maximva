#include "playfield.h"

namespace MyAscii {
    /**
     * Creates an instance of PlayField with base values
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
        this->monochromeTiles = (difficulty > 1);
        this->preventDuplicateChars = (difficulty < 3);
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
        const int GREEN_BACK_WHITE_TEXT = 0x2F;

        for (unsigned int id = 0; id < playfieldSize / setSize; id++) {
            int tileAttribute = (monochromeTiles ? GREEN_BACK_WHITE_TEXT : add_tile_color());
            Tile tile(id, tileAttribute, secret);
            int charAlphabetPosition = tile.get_flipped_char() - 'A';
            charCount[charAlphabetPosition]++;

            while (preventDuplicateChars && charCount[charAlphabetPosition] > 1) {
                add_unique_character(&tile, &charAlphabetPosition);
            }
            
            create_tile_set(&tile);
        }
        randomize_playfield();
    }


    /**
     * Add a character to the tile thats unique for this playfield
     */
    void PlayField::add_unique_character(Tile * tile, int * charAlphabetPosition) {
        charCount[*charAlphabetPosition]--;
        (*tile).set_random_char();
        (*charAlphabetPosition) = (*tile).get_flipped_char() - 'A';
        charCount[*charAlphabetPosition]++;
    }


    /**
     * Add a unique color to the tile
     */
    int PlayField::add_tile_color(void) {
        int attributeNumber = rand() % charAttributes.size();
        int attribute = charAttributes[attributeNumber];
        charAttributes.erase(std::begin(charAttributes) + attributeNumber);
        return attribute;
    }


    /**
     * Adds a set (size of a pair) to the (non-random) tile vector
     */
    void PlayField::create_tile_set(Tile * tile) {
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