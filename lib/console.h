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
    enum class ScoreCardStructure {
        TOP,
        BOTTOM,
        DOUBLE_DIVISION,
        SINGLE_DIVISION,
        EMPTY_LINE
    };

    class Console {
        public:
            Console(void);
            Console(std::string windowTitle);

        public:
            bool showPlayField(std::vector<Tile> * tiles, int fieldEdgeSize, int selectedTileX, int selectedTileY);
            bool showScoreCard(int number_of_pairs, int correct_guesses, bool stay_in_game);
            void showEndGameScreen(int number_of_pairs, int correct_guesses);
            bool showMenu(std::string items[], int items_size, int current_menu_item, bool user_input_needed);
            void showScoreTable(void);
            int getDifficulty(void);
            std::string getUserName(void);
            void toggleHiddenCharSecret(void);
            bool hiddenCharState(void);

        private:
            void create_game_screen_buffer(void);
            void print_scorecard_structure(CHAR_INFO map[], ScoreCardStructure type, int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER);
            void print_scorecard_text(const char * text, CHAR_INFO map[], int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER, int text_attribute);
            void addCharToMap(CHAR_INFO map[], int position, wchar_t character, int attribute);
            bool drawBox(HANDLE * screenBuffer, int buffer_width, int height, int width, int top_margin, bool sparkle);
            void hide_cursor(HANDLE * screenBuffer);
            void init_console_window(std::string windowTitle);
            bool showTitle(void);
            void readTitle(void);
            
        private:
            const int MENU_X_START_POSITION = 117;
            const int GAME_TOP_MARGIN = 4;
            HANDLE gameScreenBuffer;
            HANDLE defaultScreenBuffer;
            std::string windowTitle = "New window";
            int scoreCardAttribute = 0xEC;   // EC
            int difficulty = 0;
            std::string userName = "guest-player";
            bool hidden_char_secret = false;
            std::vector<std::string> title;
            wchar_t scorecard_structure_chars[5][3] = {
                {L'╔', L'═', L'╗'}, // Top
                {L'╚', L'═', L'╝'}, // Bottom
                {L'╠', L'═', L'╣'}, // Double division
                {L'╟', L'─', L'╢'}, // Single division
                {L'║', L' ', L'║'}  // Empty row
            };
            
    };
};

// Used Unicode chars for reference
        // ╔═══════════════╗
        // ║               ║
        // ╟───────────────╢
        // ╠═══════════════╣
        // ╚═══════════════╝