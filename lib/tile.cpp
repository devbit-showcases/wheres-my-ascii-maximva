#include "tile.h"

namespace MyAscii {

    Tile::Tile(unsigned int id, int charFlippedAttribute) {
        setRandomChar();
        this->id = id;
        this->charFlippedAttribute = charFlippedAttribute;
    }

    void Tile::setRandomChar(void) {
        asciiChar = rand() % ASCII_TABLE_RANGE + 1 + ASCII_TABLE_START;
    }

    char Tile::getHiddenChar(void) {
        return hiddenChar;
    }

    char Tile::getAsciiChar(void) {
        return asciiChar;
    }

    unsigned int Tile::getId(void) {
        return id;
    }

    void Tile::turnCard(void) {
        turned = !turned;
    }

    bool Tile::isTurned(void) {
        return turned;
    }

    int Tile::getCharFlippedAttribute(void) {
        return charFlippedAttribute;
    }

    int Tile::getCharCoveredAttribute(void) {
        return charCoveredAttribute;
    }

    void Tile::setHiddenChar(wchar_t hidden_char) {
        this->hiddenChar = hidden_char;
    }
    
}