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

        // std::vector<Tile> tiles;

        for (unsigned int c = 0; c < fieldSize / pair_size; c++) {
            Tile tile(c);
            for (unsigned int d = 0; d < pair_size; d++) {
                playField.push_back(tile);
            }
        }


        // for (unsigned int c = 0; c < tiles.size(); c++) {
        //     unsigned int place = rand() % tiles.size();

        //     if (tiles[place] != nullptr) {

        //     }

        //     playField.push_back( tiles[place] ); 
        // }


    }

}