#include "tile.h"

namespace MyAscii {
    /**
     * Create an instance of Tile
     */
    Tile::Tile(unsigned int id, int flippedAttribute, bool secret) {
        this->id = id;
        this->flippedAttribute = flippedAttribute;
        this->secret = secret;
        set_random_char();
    }


    /**
     * FLips the tile
     */
    void Tile::flip_tile(void) {
        turned = !turned;
    }


    /**
     * Returns if tile is flipped or not
     */
    bool Tile::is_flipped(void) {
        return turned;
    }


    /**
     * Returns the ID of a tile
     */
    unsigned int Tile::get_id(void) {
        return id;
    }


    /**
     * Returns the character to show when tile is covered
     */
    char Tile::get_covered_char(void) {
        return (secret ? secretHiddenChar : coveredChar);
    }


    /**
     * Returns the character to show when tile is flipped
     */
    char Tile::get_flipped_char(void) {
        return flippedChar;
    }


    /**
     * Get the attribute (back- and foreground color) to use when tile is flipped
     */
    int Tile::get_flipped_attribute(void) {
        return flippedAttribute;
    }


    /**
     * Get the attribute (back- and foreground color) to use when tile is covered
     */
    int Tile::get_covered_attribute(void) {
        return coveredAttribute;
    }


    /**
     * Set a random character for the tile
     */
    void Tile::set_random_char(void) {
        flippedChar = (rand() % (asciiTableRange + 1)) + asciiTableStart;
    }

}