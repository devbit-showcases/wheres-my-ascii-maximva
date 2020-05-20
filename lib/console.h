#pragma once

#include <string>
#include <windows.h>
#include <iostream>
#include "playfield.h"
#include "tile.h"
#include "score.h"
#include "player.h"
#include "scorecard.h"
#include "userinput.h"
#include "filereader.h"

namespace MyAscii {
    enum class ScoreCardStructure {
        TOP,
        BOTTOM,
        DOUBLE_DIVISION,
        SINGLE_DIVISION,
        EMPTY_LINE,     // Empty line inside box
        BLANK_LINE      // Blank line outside box
    };

    class Console {
        public:
            Console(void);
            Console(std::string windowTitle);

        public:
            void hide_cursor(HANDLE * screenBuffer);
            void show_cursor(HANDLE * screenBuffer);
            bool print_playfield(std::vector<Tile> * tiles, int fieldEdgeSize, int selectedTileX, int selectedTileY, int difficulty);
            bool show_scorecard(int number_of_pairs, int setSize, int correct_guesses, bool stay_in_game, int difficulty, std::string userName);
            bool print_menu(std::string items[], int items_size, int current_menu_item, UserInput * userInfo, bool user_input_needed);
            bool hidden_char_state(void);
            void print_endgame_screen(int number_of_pairs, int correct_guesses, double elapsedTime, Player * player);
            void showScoreTable(void);
            void showAboutPage(void);
            void toggleHiddenCharSecret(void);

        private:
            int get_screenbuffer_width(HANDLE * screenbuffer);
            int sizeof_text(const char * TEXT);
            bool draw_frame(HANDLE * screenBuffer, int buffer_width, int height, int width, int top_margin, bool sparkle);
            bool print_title(void);
            void create_textmode_screenbuffer(HANDLE * screenBuffer);
            void print_scorecard_structure(CHAR_INFO map[], ScoreCardStructure type, int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER);
            void print_scorecard_text(const char * text, CHAR_INFO map[], int NUMBER_OF_COLUMNS, int ROW_NUMBER, int text_attribute);
            void print_scorecard_text_array(const char * TEXT[], int size, CHAR_INFO map[], int numberOfColumns, int numberOfRows, int startingRow, int text_attribute);
            void add_char_to_map(CHAR_INFO map[], int position, wchar_t character, int attribute);
            void init_console_window(std::string windowTitle);
            void reset_coord(COORD * coord);
            void set_coords(COORD * coord, int xPosition, int yPosition);
            void set_x_coord(COORD * coord, int position);
            void set_y_coord(COORD * coord, int position);
            void add_menu_item_to_map(CHAR_INFO map[], const char * menuItem, bool currentMenuItem);
            void set_smallrect_position(SMALL_RECT * canvas, int top, int bottom, int left, int right);
            std::string get_timestring(double time);

        private:
            const int SCORECARD_LEFT_MARGIN = 4;
            const int SCORECARD_RIGHT_MARGIN = 1;
            const int MENU_TOP_MARGIN = 25;
            const int MENU_X_START_POSITION = 117;
            const int MENU_ITEM_HEIGHT = 3;
            const int MENU_ITEM_WIDTH = 75;
            const int MENU_ITEM_SIZE = MENU_ITEM_HEIGHT * MENU_ITEM_WIDTH;
            const int MENU_ITEM_ATTRIBUTE = 0x6F;
            const int GAME_TOP_MARGIN = 4;
            HANDLE gameScreenBuffer;
            HANDLE defaultScreenBuffer;
            std::string windowTitle = "New window";
            int scoreCardAttribute = 0xEC;   // EC
            bool hidden_char_secret = false;
            std::vector<std::string> title;
            wchar_t scorecard_structure_chars[6][3] = {
                {L'╔', L'═', L'╗'}, // Top
                {L'╚', L'═', L'╝'}, // Bottom
                {L'╠', L'═', L'╣'}, // Double division
                {L'╟', L'─', L'╢'}, // Single division
                {L'║', L' ', L'║'}, // Empty row
                {L' ', L' ', L' '}
            };
            int difficulty_tile_size[4][2] = {
                {17, 9},    // Width , Height
                {13, 7},
                {13, 7},
                {9, 5}
            };
    };
};

// Used Unicode chars for reference
        // ╔═══════════════╗
        // ║               ║
        // ╟───────────────╢
        // ╠═══════════════╣
        // ╚═══════════════╝