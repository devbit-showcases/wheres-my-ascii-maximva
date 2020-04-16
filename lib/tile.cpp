#include "tile.h"

namespace MyAscii {

    Tile::Tile(unsigned int id, int flipped_attribute) {
        setRandomChar();
        this->id = id;
        this->flipped_attribute = flipped_attribute;
    }

    void Tile::turnCard(void) {
        turned = !turned;
    }

    bool Tile::isTurned(void) {
        return turned;
    }

    // Getters
    unsigned int Tile::getId(void) {
        return id;
    }

    char Tile::getHiddenChar(void) {
        return hidden_char;
    }

    char Tile::getAsciiChar(void) {
        return flipped_char;
    }

    int Tile::getCharFlippedAttribute(void) {
        return flipped_attribute;
    }

    int Tile::getCharCoveredAttribute(void) {
        return covered_attribute;
    }

    // Setters
    void Tile::setHiddenChar(wchar_t hidden_char) {
        this->hidden_char = hidden_char;
    }

    void Tile::setRandomChar(void) {
        flipped_char = (rand() % (ASCII_TABLE_RANGE + 1)) + ASCII_TABLE_START;
    }

}