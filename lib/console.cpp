#include "Console.h"

namespace MyAscii {

    Console::Console(void)
    : Console("New window") {
    }

    Console::Console(std::string windowTitle) {
        HWND console = GetConsoleWindow();
        this->windowTitle =  windowTitle;
        SetConsoleTitleA(windowTitle.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xEC);
        ShowWindow(console, SW_MAXIMIZE);
        createGameScreenBuffer();
        createMenuScreenBuffer();

        // Hide the cursor on the game window
        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo(gameScreenBuffer, &cursorInfo);
        cursorInfo.bVisible = false; // set the cursor visibility
        SetConsoleCursorInfo(gameScreenBuffer, &cursorInfo);
    }

    void Console::createGameScreenBuffer(void) {
        gameScreenBuffer = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL);
    }

    void Console::createMenuScreenBuffer(void) {
        menuScreenBuffer = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL);
    }

    void Console::showMenu(std::string items[], int items_size, int current_menu_item) {
        CONSOLE_SCREEN_BUFFER_INFO menuBufferInfo;
        GetConsoleScreenBufferInfo(menuScreenBuffer, &menuBufferInfo);

        COORD coordinateBufferSize;
        COORD topLeftCoordinate;
        SMALL_RECT srcWriteRect;
        BOOL succes;


        for (int y = 0; y < items_size; y++) {
            const int TOP_MENU_MARGIN = 10;
            const int MENU_ITEM_HEIGHT = 3;
            const int MENU_ITEM_WIDTH = 75;
            const int MENU_SIZE = MENU_ITEM_HEIGHT * MENU_ITEM_WIDTH;
            CHAR_INFO map[MENU_SIZE];

            // Get center of screen to center menu
            int bufferWidth = menuBufferInfo.dwSize.X;
            const int START_POSITION = (bufferWidth - MENU_ITEM_WIDTH) / 2;

            for (int i = 0; i < MENU_ITEM_HEIGHT * MENU_ITEM_WIDTH; i++) {


                if (i > MENU_ITEM_WIDTH + 1 && i < (2 * MENU_ITEM_WIDTH) - 2) {
                    const char * menu_item_string = items[y].c_str();

                    int size = 0;
                    while (menu_item_string[size] != '\0') size++;

                    if (i - MENU_ITEM_WIDTH - 2 < size) {
                        map[i].Char.UnicodeChar =  menu_item_string[i - MENU_ITEM_WIDTH - 2];
                        map[i].Attributes = 0x20;
                    } else {
                        map[i].Char.UnicodeChar =  L' ';
                        map[i].Attributes = 0x20;
                    }
                } else {
                    map[i].Char.UnicodeChar = L' ';
                    map[i].Attributes = 0x20;
                }

                if (y == current_menu_item) {
                    if (i == 0 || i == MENU_ITEM_WIDTH || i == 2 * MENU_ITEM_WIDTH ) {
                        map[i].Char.UnicodeChar = L' ';
                        map[i].Attributes = 0x40;
                    }
                }
            }


            coordinateBufferSize.Y = MENU_ITEM_HEIGHT;
            coordinateBufferSize.X = MENU_ITEM_WIDTH;
            topLeftCoordinate.Y = 0;
            topLeftCoordinate.X = 0;

            (&srcWriteRect)->Top = TOP_MENU_MARGIN + (y * (MENU_ITEM_HEIGHT + 1));
            (&srcWriteRect)->Left = START_POSITION;
            (&srcWriteRect)->Bottom = TOP_MENU_MARGIN + (y * (MENU_ITEM_HEIGHT + 1)) + MENU_ITEM_HEIGHT;
            (&srcWriteRect)->Right = (START_POSITION + MENU_ITEM_WIDTH);

            succes = WriteConsoleOutputW(
                menuScreenBuffer,
                map,
                coordinateBufferSize,
                topLeftCoordinate,
                (&srcWriteRect)
            );


        }

        SetConsoleActiveScreenBuffer(menuScreenBuffer);
  
    }

    void Console::showPlayField(std::vector<Tile> * tiles, int fieldEdgeSize, int selectedTileX, int selectedTileY) {

        COORD coordinateBufferSize;
        COORD topLeftCoordinate;
        SMALL_RECT srcWriteRect;
        BOOL succes;

        const int START_POSITION = (117 - (fieldEdgeSize * 9) - ((fieldEdgeSize - 1) * 2)) / 2;

        for (int y = 0; y < fieldEdgeSize; y++) {
            for (int x = 0; x < fieldEdgeSize; x++) {
                // 1 tile is 9 vertical and 5 horizontal characters
                const int MAP_SIZE = 45;
                CHAR_INFO map[MAP_SIZE];
                const int TILE_ARRAY_INDEX = x + (y * fieldEdgeSize);
                const int TILE_CENTER = 22;

                // Display playfield on screen
                for (int j = 0; j < 45; j++) {

                    const char CHAR_TO_GUESS = ((*tiles)[TILE_ARRAY_INDEX].isTurned() ? (*tiles)[TILE_ARRAY_INDEX].getAsciiChar() : (*tiles)[TILE_ARRAY_INDEX].getHiddenChar()); // ascii : hidden
                    const int TILE_FLIPPED_ATTRIBUTE = (*tiles)[TILE_ARRAY_INDEX].getCharFlippedAttribute();
                    const int TILE_COVERED_ATTRIBUTE = (*tiles)[TILE_ARRAY_INDEX].getCharCoveredAttribute();
                    const int TILE_SHOW_ATTRIBUTE = ((*tiles)[TILE_ARRAY_INDEX].isTurned() ? TILE_FLIPPED_ATTRIBUTE : TILE_COVERED_ATTRIBUTE); // Flipped : Covered

                    if (x == selectedTileX && y == selectedTileY) {
                        if (j == 11) {
                            map[j].Char.UnicodeChar = L'┌';
                        } else if ((j > 11 && j < 15) || (j > 29 && j < 33)){
                            map[j].Char.UnicodeChar = L'─';
                        } else if (j == 15) {
                            map[j].Char.UnicodeChar = L'┐';
                        } else if (j == 20 || j == 24) {
                            map[j].Char.UnicodeChar = L'│';
                        } else if (j == 29) {
                            map[j].Char.UnicodeChar = L'└';
                        } else if (j == 33) {
                            map[j].Char.UnicodeChar = L'┘';
                        } else if (j == TILE_CENTER) {
                            map[j].Char.UnicodeChar = CHAR_TO_GUESS;
                        } else {
                            map[j].Char.UnicodeChar = L' ';
                        }
                    } else {
                        map[j].Char.UnicodeChar = (j == TILE_CENTER ? CHAR_TO_GUESS : L' ');     // Use   L'▓' for unicode chars
                    }

                    map[j].Attributes = TILE_SHOW_ATTRIBUTE;
                }

                coordinateBufferSize.Y = 5;
                coordinateBufferSize.X = 9;
                topLeftCoordinate.Y = 0;
                topLeftCoordinate.X = 0;

                // TODO CHECK MATH AND SIMPLIFY - make all of this depend on width/height/border of tiles
                const int TILE_VERTICAL_WIDTH = 4;
                const int TILE_VERTICAL_BORDER = 2;

                (&srcWriteRect)->Top = ((TILE_VERTICAL_WIDTH + TILE_VERTICAL_BORDER) * y) + TILE_VERTICAL_WIDTH;
                (&srcWriteRect)->Left = START_POSITION + (x * 11);          // Beginning of centered tile field
                (&srcWriteRect)->Bottom = (6 * y) + 8;
                (&srcWriteRect)->Right = START_POSITION + 8 + (x * 11);     // End of centered tile field

                succes = WriteConsoleOutputW(
                    gameScreenBuffer,
                    map,
                    coordinateBufferSize,
                    topLeftCoordinate,
                    (&srcWriteRect)
                );
            }
        }

        SetConsoleActiveScreenBuffer(gameScreenBuffer);
    }

    void Console::showScoreCard(int number_of_pairs, int correct_guesses) {
        COORD coordinateBufferSize;
        COORD topLeftCoordinate;
        SMALL_RECT srcWriteRect;
        BOOL succes;

        const int NUMBER_OF_ROWS = 10;
        const int NUMBER_OF_COLUMNS = 50;
        const int START_X_POSITION = 117;
        const int START_Y_POSITION = 4;
        const int SCORECARD_SIZE = NUMBER_OF_ROWS * NUMBER_OF_COLUMNS;
        CHAR_INFO map[SCORECARD_SIZE];

        // Draw the empty scorecard box
        drawScorecardTopAndBottom(map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS);
        for (int i = 1; i < NUMBER_OF_ROWS - 1; i++) {
            drawScoreCardEmptyRow(map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, i);
        }

        // Draw the score
        drawScoreCardScore(map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 2, correct_guesses, number_of_pairs);

        // Get ready to write it to the console screenBuffer
        coordinateBufferSize.Y = NUMBER_OF_ROWS;
        coordinateBufferSize.X = NUMBER_OF_COLUMNS;
        topLeftCoordinate.Y = 0;
        topLeftCoordinate.X = 0;

        (&srcWriteRect)->Top = START_Y_POSITION;
        (&srcWriteRect)->Left = START_X_POSITION;
        (&srcWriteRect)->Bottom = START_Y_POSITION + NUMBER_OF_ROWS;
        (&srcWriteRect)->Right = START_X_POSITION + NUMBER_OF_COLUMNS;

        // Write it to the console screenBuffer
        succes = WriteConsoleOutputW(
            gameScreenBuffer,
            map,
            coordinateBufferSize,
            topLeftCoordinate,
            (&srcWriteRect)
        );
    }

    void Console::drawScoreCardScore(CHAR_INFO map[], int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER, int SCORE, int MAX_SCORE) {
        const char * max_score = std::to_string(MAX_SCORE).c_str();
        const char * score = std::to_string(SCORE).c_str();
        const int LEFT_MARGIN = 4; // 1 for border + actual margin
        const int RIGHT_MARGIN = 1; // 1 for border + actual margin
        const int START_POSITION = (NUMBER_OF_COLUMNS * ROW_NUMBER) + LEFT_MARGIN;
        const int END_OF_LINE = (NUMBER_OF_COLUMNS * (ROW_NUMBER + 1)) - RIGHT_MARGIN;

        int count = 0;
        
        for (int i = START_POSITION; i < END_OF_LINE; i++) {
            if (!(count > sizeof(score)/sizeof(int))) {
                addCharToMap(map, i, score[count], scoreCardAttribute);
                count++;
            } else {
                addCharToMap(map, i, L' ', scoreCardAttribute);
            }
        }
    }

    void Console::drawScoreCardEmptyRow(CHAR_INFO map[], int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER) {
        for (int y = 0; y < NUMBER_OF_ROWS; y++) {
            for (int x = 0; x < NUMBER_OF_COLUMNS; x++) {
                const int MAP_POSITION = x + (y * NUMBER_OF_COLUMNS);
                if (MAP_POSITION == NUMBER_OF_COLUMNS * ROW_NUMBER) {
                    addCharToMap(map, MAP_POSITION, L'║', scoreCardAttribute);
                } else if (MAP_POSITION > NUMBER_OF_COLUMNS * ROW_NUMBER && MAP_POSITION < (NUMBER_OF_COLUMNS * ROW_NUMBER) + (NUMBER_OF_COLUMNS - 1)) {
                    addCharToMap(map, MAP_POSITION, L' ', scoreCardAttribute);
                } else if (MAP_POSITION == (NUMBER_OF_COLUMNS * ROW_NUMBER) + (NUMBER_OF_COLUMNS - 1)) {
                    addCharToMap(map, MAP_POSITION, L'║', scoreCardAttribute);
                }
            }
        }
    }

    void Console::drawScorecardTopAndBottom(CHAR_INFO map[], int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS) {
        const int FIRST_ROW_START = 0;
        const int FIRST_ROW_END = (NUMBER_OF_COLUMNS - 1);
        const int FINAL_ROW_START = (NUMBER_OF_COLUMNS * (NUMBER_OF_ROWS - 1));
        const int FINAL_ROW_END = (NUMBER_OF_COLUMNS * NUMBER_OF_ROWS) - 1;

        // Used Unicode chars for reference
        //╔═══════════════╗
        //║               ║
        //╟───────────────╢
        //╠═══════════════╣
        //╚═══════════════╝

        for (int y = 0; y < NUMBER_OF_ROWS; y++) {
            for (int x = 0; x < NUMBER_OF_COLUMNS; x++) {
                const int MAP_POSITION = x + (y * NUMBER_OF_COLUMNS);
                // Draw first row of scorecard
                if (MAP_POSITION == FIRST_ROW_START) {
                    addCharToMap(map, MAP_POSITION, L'╔', scoreCardAttribute);
                } else if (MAP_POSITION > FIRST_ROW_START && MAP_POSITION < FIRST_ROW_END) {
                    addCharToMap(map, MAP_POSITION, L'═', scoreCardAttribute);
                } else if(MAP_POSITION == FIRST_ROW_END) {
                    addCharToMap(map, MAP_POSITION, L'╗', scoreCardAttribute);
                }
                // Draw final row of scorecard
                if (MAP_POSITION == FINAL_ROW_START) {
                    addCharToMap(map, MAP_POSITION, L'╚', scoreCardAttribute);
                } else if (MAP_POSITION > FINAL_ROW_START && MAP_POSITION < FINAL_ROW_END) {
                    addCharToMap(map, MAP_POSITION, L'═', scoreCardAttribute);
                } else if(MAP_POSITION == FINAL_ROW_END) {
                    addCharToMap(map, MAP_POSITION, L'╝', scoreCardAttribute);
                }
            }
        }
    }

    void Console::addCharToMap(CHAR_INFO map[], int position, wchar_t character, int attribute) {
        map[position].Char.UnicodeChar = character;
        map[position].Attributes = attribute;
    }

    void Console::showMenuItem(CHAR_INFO map[], int position, std::string menu_item) {

        const int MENU_ITEM_WIDTH = 50;
        const int MENU_ITEM_HEIGHT = 3;

        const int MENU_LEFT_START = (100 - MENU_ITEM_WIDTH ) / 2;

        for (int y = 4 + (position * (MENU_ITEM_HEIGHT + 1)); y < MENU_ITEM_HEIGHT; y++) {
            for (int x = MENU_LEFT_START; x < MENU_LEFT_START + MENU_ITEM_WIDTH; x++) {

                const int POSITION = x + (y * MENU_ITEM_WIDTH);

                map[POSITION].Char.UnicodeChar = L'┌';
                map[POSITION].Attributes = 0x20;
            }
        }
            

    }

}