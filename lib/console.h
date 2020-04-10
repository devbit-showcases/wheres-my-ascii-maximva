#pragma once

#include <string>
#include <windows.h>
#include <iostream>
#include "playfield.h"
#include "tile.h"

// To try and display UNICODE chars
#include <io.h>
#include <fcntl.h>

namespace MyAscii {

    class Console {
        public:
            Console(void);
            Console(std::string windowTitle);

        public:
            void showPlayField(std::vector<Tile> * tiles, int fieldEdgeSize, int selectedTileX, int selectedTileY);
            void showScoreCard(int number_of_pairs, int correct_guesses);
            void showMenu(std::string items[], int items_size, int current_menu_item);

        private:
            void createGameScreenBuffer(void);
            void createMenuScreenBuffer(void);
            void drawScorecardTopAndBottom(CHAR_INFO map[], int number_of_columns, int number_of_rows);
            void drawScoreCardEmptyRow(CHAR_INFO map[], int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER);
            void drawScoreCardScore(CHAR_INFO map[], int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER, int score, int max_score);
            void addCharToMap(CHAR_INFO map[], int position, wchar_t character, int attribute);
            void showMenuItem(CHAR_INFO map[], int position, std::string menu_item);
            
        private:
            std::string windowTitle = "New window";
            HANDLE gameScreenBuffer;
            HANDLE menuScreenBuffer;
            int scoreCardAttribute = 0xEC;   // EC

    };
};