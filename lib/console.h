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
        EMPTY_LINE,     // Empty line inside box
        BLANK_LINE      // Blank line outside box
    };

    class Console {
        public:
            Console(void);
            Console(std::string windowTitle);

        public:
            bool showPlayField(std::vector<Tile> * tiles, int fieldEdgeSize, int selectedTileX, int selectedTileY);
            bool showScoreCard(int number_of_pairs, int correct_guesses, bool stay_in_game);
            void print_endgame_screen(int number_of_pairs, int correct_guesses);
            bool showMenu(std::string items[], int items_size, int current_menu_item, bool user_input_needed);
            void showScoreTable(void);
            void showAboutPage(void);
            int getDifficulty(void);
            std::string getUserName(void);
            void toggleHiddenCharSecret(void);
            bool hiddenCharState(void);
            void hide_cursor(HANDLE * screenBuffer);

        private:
            void create_generic_screenbuffer(HANDLE * screenBuffer);
            void print_scorecard_structure(CHAR_INFO map[], ScoreCardStructure type, int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER);
            void print_scorecard_text(const char * text, CHAR_INFO map[], int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER, int text_attribute);
            void add_char_to_map(CHAR_INFO map[], int position, wchar_t character, int attribute);
            bool draw_frame(HANDLE * screenBuffer, int buffer_width, int height, int width, int top_margin, bool sparkle);
            void init_console_window(std::string windowTitle);
            bool show_title(void);
            std::vector<std::string> read_textfile(std::string filename);
            void reset_coord(COORD * coord);
            void set_coords(COORD * coord, int xPosition, int yPosition);
            void set_x_coord(COORD * coord, int position);
            void set_y_coord(COORD * coord, int position);
            int get_screenbuffer_width(HANDLE * screenbuffer);
            int sizeof_text(const char * TEXT);

        private:
            // Margin and padding values
            const int SCORECARD_LEFT_MARGIN = 4;
            const int SCORECARD_RIGHT_MARGIN = 1;
            const int MENU_TOP_MARGIN = 25;
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