#pragma once

#include <string>
#include <windows.h>
#include <iostream>
#include "playfield.h"
#include "tile.h"
#include "score.h"
#include "player.h"
#include "scorecard.h"

namespace MyAscii {

    class Console {
        public:
            Console(void);
            Console(std::string windowTitle);

        public:
            bool showPlayField(std::vector<Tile> * tiles, int fieldEdgeSize, int selectedTileX, int selectedTileY);
            bool showScoreCard(int number_of_pairs, int correct_guesses, bool stay_in_game);
            bool showMenu(std::string items[], int items_size, int current_menu_item, bool user_input_needed);
            void showScoreTable(void);
            int getDifficulty(void);
            std::string getUserName(void);
            void toggleHiddenCharSecret(void);
            bool hiddenCharState(void);
            void showTitle(void);

        private:
            void createGameScreenBuffer(void);
            void drawScorecardTopAndBottom(CHAR_INFO map[], int number_of_columns, int number_of_rows);
            void drawScoreCardEmptyRow(CHAR_INFO map[], int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER);
            void drawScoreCardScore(CHAR_INFO map[], int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER, int score, int max_score);
            void drawScoreCardPlayerName(CHAR_INFO map[], int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER);
            void addCharToMap(CHAR_INFO map[], int position, wchar_t character, int attribute);
            void showMenuItem(CHAR_INFO map[], int position, std::string menu_item);
            bool drawBox(HANDLE * screenBuffer, int buffer_width, int height, int width, int top_margin, bool sparkle);
            
        private:
            HANDLE gameScreenBuffer;
            HANDLE defaultScreenBuffer;
            std::string windowTitle = "New window";
            int scoreCardAttribute = 0xEC;   // EC
            int difficulty = 0;
            std::string userName = "guest-player";
            bool hidden_char_secret = false;
            std::vector<std::string> title;
    };
};