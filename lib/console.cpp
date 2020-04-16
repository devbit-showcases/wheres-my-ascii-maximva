#include "Console.h"

namespace MyAscii {

    Console::Console(void)
    : Console("New window") {
    }

    Console::Console(std::string windowTitle) {
        init_console_window(windowTitle);
        defaultScreenBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
        create_game_screen_buffer();
        readTitle();
    }

    void Console::init_console_window(std::string windowTitle) {
        // Set title, background color and go full-screen
        HWND console = GetConsoleWindow();
        this->windowTitle =  windowTitle;
        SetConsoleTitleA(windowTitle.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xEC);
        ShowWindow(console, SW_MAXIMIZE); // Go to full-screen
    }

    void Console::hide_cursor(HANDLE * screenBuffer) {
        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo((*screenBuffer), &cursorInfo);
        cursorInfo.bVisible = false; // set the cursor visibility
        SetConsoleCursorInfo((*screenBuffer), &cursorInfo);
    }

    void Console::create_game_screen_buffer(void) { 
        gameScreenBuffer = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL);
        hide_cursor(&gameScreenBuffer);
    }

    int Console::getDifficulty(void) {
        return difficulty;
    }

    std::string Console::getUserName(void) {
        return userName;
    }

    void Console::toggleHiddenCharSecret(void) {
        hidden_char_secret = !hidden_char_secret;
    }

    bool Console::hiddenCharState(void) {
        return hidden_char_secret;
    }

    void Console::readTitle(void) {
        // Read title art from file and store it
        std::ifstream title_file;
        title_file.open("title.txt");
        if (title_file.is_open()) {
            std::string part_title;
            while (getline(title_file, part_title)){
                title.push_back(part_title);
            }
        }
    }

    bool Console::showTitle(void) {
        CONSOLE_SCREEN_BUFFER_INFO defaultBufferInfo;
        GetConsoleScreenBufferInfo(defaultScreenBuffer, &defaultBufferInfo);
        COORD coordinateBufferSize;
        COORD topLeftCoordinate;
        SMALL_RECT titleRect;
        BOOL succes;

        int bufferWidth = defaultBufferInfo.dwSize.X;
        int title_card_attribute = 0xEC;
        const int TITLE_CARD_WIDTH = 130;
        const int TITLE_CARD_HEIGHT = 16;
        const int TITLE_CARD_START_POSITION = (bufferWidth - TITLE_CARD_WIDTH) / 2;
        const int TOP_MARGIN = 4;
        const int LEFT_MARGIN = (TITLE_CARD_WIDTH - title[0].length()) / 2;

        // Show frame for title and how-to info
        CHAR_INFO title_card[TITLE_CARD_WIDTH * TITLE_CARD_HEIGHT];
        for (unsigned int y = 0; y < TITLE_CARD_HEIGHT; y++) {
            for (unsigned int x = 0; x < TITLE_CARD_WIDTH; x++) {
                wchar_t fill_char = (x == 0 || x == 1 || x == TITLE_CARD_WIDTH - 2 || x == TITLE_CARD_WIDTH - 1 || y == 0 || y == 10 || y == TITLE_CARD_HEIGHT - 1 ? L'░' : L' ');
                title_card[x + (y * TITLE_CARD_WIDTH)].Char.UnicodeChar = fill_char;
                title_card[x + (y * TITLE_CARD_WIDTH)].Attributes = title_card_attribute;
            }
        }

        coordinateBufferSize.Y = TITLE_CARD_HEIGHT;
        coordinateBufferSize.X = TITLE_CARD_WIDTH;
        topLeftCoordinate.Y = 0;
        topLeftCoordinate.X = 0;

        (&titleRect)->Top = TOP_MARGIN;
        (&titleRect)->Left = TITLE_CARD_START_POSITION;
        (&titleRect)->Bottom = TOP_MARGIN + TITLE_CARD_HEIGHT;
        (&titleRect)->Right = TITLE_CARD_START_POSITION + TITLE_CARD_WIDTH;

        succes = WriteConsoleOutputW(
            defaultScreenBuffer,
            title_card,
            coordinateBufferSize,
            topLeftCoordinate,
            (&titleRect)
        );

        // Print the title to the console
        COORD title_coord;
        title_coord.X = TITLE_CARD_START_POSITION + LEFT_MARGIN;
        title_coord.Y = TOP_MARGIN + 2;
        SetConsoleCursorPosition(defaultScreenBuffer, title_coord);

        for (unsigned int i = 0; i < 6; i++) {  // for some reason tile.size() doesn't, reprints title every time...
            std::cout << title[i];
            title_coord.Y++;
            SetConsoleCursorPosition(defaultScreenBuffer, title_coord);
        }

        title_coord.Y += 4;
        SetConsoleCursorPosition(defaultScreenBuffer, title_coord);
        std::cout << "Use the [UP] and [DOWN] arrow keys to navigate, [ENTER] to select.";

        // Reset cursor position
        title_coord.X = 0;
        title_coord.Y = 0;
        SetConsoleCursorPosition(defaultScreenBuffer, title_coord);

        return succes;
    }

    bool Console::showMenu(std::string items[], int items_size, int current_menu_item, bool user_input_needed) {
        if (user_input_needed) {
            system("CLS");
        }
        showTitle();

        CONSOLE_SCREEN_BUFFER_INFO defaultBufferInfo;
        GetConsoleScreenBufferInfo(defaultScreenBuffer, &defaultBufferInfo);
        COORD coordinateBufferSize;
        COORD topLeftCoordinate;
        SMALL_RECT srcWriteRect;
        BOOL succes;

        int bufferWidth = defaultBufferInfo.dwSize.X;
        const int TOP_MENU_MARGIN = 25;
        const int MENU_ITEM_HEIGHT = 3;
        const int MENU_ITEM_WIDTH = 75;
        const int MENU_SIZE = MENU_ITEM_HEIGHT * MENU_ITEM_WIDTH;
        const int START_POSITION = (bufferWidth - MENU_ITEM_WIDTH) / 2;
        const int MENU_ITEM_ATTRIBUTE = 0x6F;
        
        // Print menu items to console
        for (int y = 0; y < items_size; y++) {
            CHAR_INFO map[MENU_SIZE];

            for (int i = 0; i < MENU_ITEM_HEIGHT * MENU_ITEM_WIDTH; i++) {

                if (i > MENU_ITEM_WIDTH + 1 && i < (2 * MENU_ITEM_WIDTH) - 2) {
                    const char * menu_item_string = items[y].c_str();

                    int size = 0;
                    while (menu_item_string[size] != '\0') size++;

                    if (i - MENU_ITEM_WIDTH - 2 < size) {
                        map[i].Char.UnicodeChar =  menu_item_string[i - MENU_ITEM_WIDTH - 2];
                        map[i].Attributes = MENU_ITEM_ATTRIBUTE;
                    } else {
                        map[i].Char.UnicodeChar =  L' ';
                        map[i].Attributes = MENU_ITEM_ATTRIBUTE;
                    }
                } else {
                    map[i].Char.UnicodeChar = L' ';
                    map[i].Attributes = MENU_ITEM_ATTRIBUTE;
                }

                if (y == current_menu_item) {
                    if (i == 0 || i == MENU_ITEM_WIDTH || i == 2 * MENU_ITEM_WIDTH ) {
                        map[i].Char.UnicodeChar = L' ';
                        map[i].Attributes = 0x40;
                    }
                }
            }

            const int USER_INPUT_SPACER = ((user_input_needed) && (y == 1 || y == 2) ? 5 : 0);

            coordinateBufferSize.Y = MENU_ITEM_HEIGHT;
            coordinateBufferSize.X = MENU_ITEM_WIDTH;
            topLeftCoordinate.Y = 0;
            topLeftCoordinate.X = 0;

            (&srcWriteRect)->Top = USER_INPUT_SPACER + TOP_MENU_MARGIN + (y * (MENU_ITEM_HEIGHT + 1));
            (&srcWriteRect)->Left = START_POSITION;
            (&srcWriteRect)->Bottom = USER_INPUT_SPACER + TOP_MENU_MARGIN + (y * (MENU_ITEM_HEIGHT + 1)) + MENU_ITEM_HEIGHT;
            (&srcWriteRect)->Right = (START_POSITION + MENU_ITEM_WIDTH);

            succes = WriteConsoleOutputW(
                defaultScreenBuffer,
                map,
                coordinateBufferSize,
                topLeftCoordinate,
                (&srcWriteRect)
            );
        }

        SetConsoleActiveScreenBuffer(defaultScreenBuffer);
        if (user_input_needed) {
            COORD cursorCoord;
            cursorCoord.X = START_POSITION + 1;
            cursorCoord.Y = TOP_MENU_MARGIN + MENU_ITEM_HEIGHT + 1;
            do {
                SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
                std::cout << "Who's playing? ";
                std::getline(std::cin, userName);
            } while (userName.length() == 0);

            // cursorCoord.X = START_POSITION + 1;
            cursorCoord.Y += 2;
            bool is_number = false;
            do {
                SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
                std::cout << "What can you handle (1 - 4)? ";
                std::cin >> difficulty;

                if (std::cin.good()){
                    is_number = true;
                } else {
                    std::cin.clear();
                    std::cin.ignore(100000,'\n');
                }
    
            } while (!is_number || difficulty == 0 || difficulty > 4);
            difficulty--; // Zero indexed for internal use
            system("CLS"); // Clear screen before leaving, otherwise menu looks funny when exiting game
        }
        return succes;
    }

    bool Console::showPlayField(std::vector<Tile> * tiles, int fieldEdgeSize, int selectedTileX, int selectedTileY) {
        COORD coordinateBufferSize;
        COORD topLeftCoordinate;
        SMALL_RECT playfieldRect;
        BOOL succes;

        const int TILE_WIDTH = 9;   // Has to be an uneven number  9 (min), 13, 17 (no exact center with even number)
        const int TILE_HEIGHT = 5;  // Has to be an uneven number  5 (min), 7, 9
        const int MAP_SIZE = TILE_WIDTH * TILE_HEIGHT;
        const int TILE_CENTER = (MAP_SIZE - 1) / 2;
        const int TOP_MARGIN = 4;
        const int VERTICAL_SPACING = 1;
        const int HORIZONTAL_SPACING = 2;
        const int COLUMN_START_OF_MENU = 117;
        const int START_POSITION = (COLUMN_START_OF_MENU - (fieldEdgeSize * TILE_WIDTH) - ((fieldEdgeSize - 1) * 2)) / 2;

        for (int y = 0; y < fieldEdgeSize; y++) {
            for (int x = 0; x < fieldEdgeSize; x++) {
                CHAR_INFO map[MAP_SIZE];
                const int TILE_ARRAY_INDEX = x + (y * fieldEdgeSize);

                // Display playfield on screen
                for (int j = 0; j < MAP_SIZE; j++) {
                    const wchar_t CHAR_TO_GUESS = ((*tiles)[TILE_ARRAY_INDEX].isTurned() ? (*tiles)[TILE_ARRAY_INDEX].getAsciiChar() : (*tiles)[TILE_ARRAY_INDEX].getHiddenChar()); // ascii : hidden
                    const int TILE_FLIPPED_ATTRIBUTE = (*tiles)[TILE_ARRAY_INDEX].getCharFlippedAttribute();
                    const int TILE_COVERED_ATTRIBUTE = (*tiles)[TILE_ARRAY_INDEX].getCharCoveredAttribute();
                    const int TILE_SHOW_ATTRIBUTE = ((*tiles)[TILE_ARRAY_INDEX].isTurned() ? TILE_FLIPPED_ATTRIBUTE : TILE_COVERED_ATTRIBUTE); // Flipped : Covered

                    if (x == selectedTileX && y == selectedTileY) {
                        if (j == (TILE_WIDTH + 2)) {
                            map[j].Char.UnicodeChar = L'┌';
                        } else if ((j > TILE_WIDTH + 2 && j < (2 * TILE_WIDTH) - 3) || ((j > MAP_SIZE - (2 * TILE_WIDTH) + 2) && j < (MAP_SIZE - TILE_WIDTH - 3))){
                            map[j].Char.UnicodeChar = L'─';
                        } else if (j == (2 * TILE_WIDTH) - 3) {
                            map[j].Char.UnicodeChar = L'┐';  // || j % (2 * TILE_WIDTH) - 3)
                        } else if (j > ((2 * TILE_WIDTH) - 3 ) && j < (MAP_SIZE - (2 * TILE_WIDTH) + 2) && ((j - 2) % (TILE_WIDTH) == 0 || (j + 3) % TILE_WIDTH == 0)) {   
                            map[j].Char.UnicodeChar = L'│';
                        } else if (j == MAP_SIZE - (2 * TILE_WIDTH) + 2) {
                            map[j].Char.UnicodeChar = L'└';
                        } else if (j == MAP_SIZE - TILE_WIDTH - 3) {
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

                coordinateBufferSize.Y = TILE_HEIGHT;
                coordinateBufferSize.X = TILE_WIDTH;
                topLeftCoordinate.Y = 0;
                topLeftCoordinate.X = 0;

                (&playfieldRect)->Top = TOP_MARGIN + (y * (TILE_HEIGHT + VERTICAL_SPACING));
                (&playfieldRect)->Left = START_POSITION + (x * (TILE_WIDTH + HORIZONTAL_SPACING));
                (&playfieldRect)->Bottom = TOP_MARGIN + (y * (TILE_HEIGHT + VERTICAL_SPACING)) + TILE_HEIGHT;
                (&playfieldRect)->Right = START_POSITION + (x * (TILE_WIDTH + HORIZONTAL_SPACING)) + TILE_WIDTH;

                succes = WriteConsoleOutputW(
                    gameScreenBuffer,
                    map,
                    coordinateBufferSize,
                    topLeftCoordinate,
                    (&playfieldRect)
                );
            }
        }
        
        SetConsoleActiveScreenBuffer(gameScreenBuffer);
        return succes;
    }

    void Console::showScoreTable(void) {
        system("CLS");
        std::vector<Score> scores;
        ScoreCard scorecard;
        scorecard.get_scoretable(&scores);
        const int LEFT_PADDING = 20;
        const int TOP_MARGIN = 10;
        const int TOP_PADDING = 7;
        const int FRAME_WIDTH = 130;
        const int FRAME_HEIGHT = 40;
        const int MAX_NAME_SIZE = 15;

        CONSOLE_SCREEN_BUFFER_INFO defaultBufferInfo;
        GetConsoleScreenBufferInfo(defaultScreenBuffer, &defaultBufferInfo);
        int bufferWidth = defaultBufferInfo.dwSize.X;

        const int START_POSITION = (bufferWidth - FRAME_WIDTH) / 2;
        const int NAME_COLUMN_OFFSET = 10;
        const int SCORE_COLUMN_OFFSET = 17;
        const int TIME_COLUMN_OFFSET = 20;
        const int MINUTE_MARK_OFFSET = 5;
        const int DIFFICULTY_COLUMN_OFFSET = 20;

        COORD cursorCoord;
        cursorCoord.Y = TOP_MARGIN + TOP_PADDING;
        cursorCoord.X = START_POSITION + LEFT_PADDING;

        SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
        std::cout << "Ranking";

        cursorCoord.X += NAME_COLUMN_OFFSET;
        SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
        std::cout << "Player name";

        cursorCoord.X += SCORE_COLUMN_OFFSET;
        SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
        std::cout << "Tile-sets found";

        cursorCoord.X += TIME_COLUMN_OFFSET;
        SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
        std::cout << "Time it took";

        cursorCoord.X += DIFFICULTY_COLUMN_OFFSET + MINUTE_MARK_OFFSET;
        SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
        std::cout << "Difficulty level";
        cursorCoord.Y += 2;

        for (unsigned int i = 0; i < scores.size() && i < 15; i++) {
            cursorCoord.X = START_POSITION + LEFT_PADDING + 1;
            SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
            std::cout << (i + 1) << ".";

            cursorCoord.X += NAME_COLUMN_OFFSET;
            SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
            std::string name = scores[i].get_name();
            if (name.length() > MAX_NAME_SIZE) name = name.substr(0, MAX_NAME_SIZE); // Limit name size to allotted size in table
            std::cout << name;

            cursorCoord.X += SCORE_COLUMN_OFFSET;
            SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
            std::cout << scores[i].get_correct_guesses() << "/" << scores[i].get_number_of_sets();

            cursorCoord.X += TIME_COLUMN_OFFSET;
            SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
            int minutes = scores[i].get_elapsed_time() / 60;
            int tens_seconds = (scores[i].get_elapsed_time() - (minutes * 60)) / 10; // % Doesn't work for some reason
            int hundreds_seconds = (scores[i].get_elapsed_time() - (minutes * 60) - (tens_seconds * 10));
            std::cout << minutes << "." << tens_seconds << hundreds_seconds;
            cursorCoord.X += MINUTE_MARK_OFFSET;
            SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
            std::cout << " min.";

            cursorCoord.X += DIFFICULTY_COLUMN_OFFSET;
            SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
            std::cout << "(difficulty " << (scores[i].get_difficulty() + 1) << ")" << std::endl;

            cursorCoord.Y++;
        }

        cursorCoord.X = START_POSITION + LEFT_PADDING;
        cursorCoord.Y += 7;
        SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
        std::cout << "Press [ENTER] to go back to the main menu.";

        drawBox(&defaultScreenBuffer, bufferWidth, FRAME_HEIGHT, FRAME_WIDTH, TOP_MARGIN, true);
    }

    bool Console::showScoreCard(int number_of_pairs, int correct_guesses, bool stay_in_game) {
        CONSOLE_SCREEN_BUFFER_INFO defaultBufferInfo;
        GetConsoleScreenBufferInfo(defaultScreenBuffer, &defaultBufferInfo);
        int bufferWidth = defaultBufferInfo.dwSize.X;

        COORD coordinateBufferSize;
        COORD topLeftCoordinate;
        SMALL_RECT srcWriteRect;
        BOOL succes;

        if (stay_in_game) {
            const int PLAYER_NAME_ROW_NUMBER = 2;
            const int SCORE_ROW_NUMBER = 3;

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
            drawScoreCardPlayerName(map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, PLAYER_NAME_ROW_NUMBER);
            drawScoreCardScore(map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, SCORE_ROW_NUMBER, correct_guesses, number_of_pairs);

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
        } else {
            const int SCORE_CARD_WIDTH = 70;
            const int SCORE_CARD_HEIGHT = 16;
            const int TOP_MARGIN = 15;
            const int LEFT_PADDING = 10;
            const int TOP_PADDING = 5;
            const int START_POSITION = (bufferWidth - SCORE_CARD_WIDTH) / 2;
            bool sparkle = (correct_guesses == number_of_pairs);

            std::string game_over_text[] = {
                "you didn't even try ...",
                "try harder next time?",
                "nice job!"
            };
            std::string found_plural = (correct_guesses == number_of_pairs ? " You found all " : " You found ");
            std::string out_of_plural = (correct_guesses == number_of_pairs ? "" : " out of " + std::to_string(number_of_pairs));

            int game_score_level;
            if (correct_guesses == number_of_pairs) {
                game_score_level = 2;
            } else if (correct_guesses == 0) {
                game_score_level = 0;
            } else {
                game_score_level = 1;
            }

            // Output game summary to console screen
            COORD cursorCoord;
            cursorCoord.X = START_POSITION + LEFT_PADDING;
            cursorCoord.Y = TOP_MARGIN + TOP_PADDING;

            SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
            std::cout << "Hi " << userName << ",";

            cursorCoord.Y += 1;
            SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
            std::cout << game_over_text[game_score_level] << found_plural << std::to_string(correct_guesses) << out_of_plural << " sets!";

            cursorCoord.Y += 2;
            SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
            std::cout << "Press [ENTER] to go back to the main menu.";

            // After outputting the text, draw a box around it
            drawBox(&defaultScreenBuffer, bufferWidth, SCORE_CARD_HEIGHT, SCORE_CARD_WIDTH, TOP_MARGIN, sparkle);
        }
        return succes;
    }

    bool Console::drawBox(HANDLE * screenBuffer, int buffer_width, int height, int width, int top_margin, bool sparkle) {
        const int SCORE_CARD_WIDTH = width;
        const int SCORE_CARD_HEIGHT = height;
        const int START_POSITION = (buffer_width - SCORE_CARD_WIDTH) / 2;
        const int HORIZONTAL_BORDER = 1;
        const int VERTICAL_BORDER = 2;
        const int TOP_MARGIN = top_margin;
        BOOL succes;
        
        COORD topLeftCoordinate;
        topLeftCoordinate.Y = 0;
        topLeftCoordinate.X = 0;

        COORD horizontal_buffer_size;
        horizontal_buffer_size.X = SCORE_CARD_WIDTH;
        horizontal_buffer_size.Y = HORIZONTAL_BORDER;

        COORD vertical_buffer_size;
        vertical_buffer_size.X = VERTICAL_BORDER;
        vertical_buffer_size.Y = SCORE_CARD_HEIGHT;
        
        // Set position of all frame edges on screen
        SMALL_RECT topRect;
        (&topRect)->Top = TOP_MARGIN;
        (&topRect)->Left = START_POSITION;
        (&topRect)->Bottom = TOP_MARGIN;
        (&topRect)->Right = START_POSITION + SCORE_CARD_WIDTH;
        SMALL_RECT bottomRect;
        (&bottomRect)->Top = TOP_MARGIN + SCORE_CARD_HEIGHT - 1;
        (&bottomRect)->Left = START_POSITION;
        (&bottomRect)->Bottom = TOP_MARGIN + SCORE_CARD_HEIGHT - 1;
        (&bottomRect)->Right = START_POSITION + SCORE_CARD_WIDTH;
        SMALL_RECT leftRect;
        (&leftRect)->Top = TOP_MARGIN;
        (&leftRect)->Left = START_POSITION;
        (&leftRect)->Bottom = TOP_MARGIN + SCORE_CARD_HEIGHT;
        (&leftRect)->Right = START_POSITION + 1;
        SMALL_RECT rightRect;
        (&rightRect)->Top = TOP_MARGIN;
        (&rightRect)->Left = START_POSITION + SCORE_CARD_WIDTH - 1;
        (&rightRect)->Bottom = TOP_MARGIN + SCORE_CARD_HEIGHT;
        (&rightRect)->Right = START_POSITION + SCORE_CARD_WIDTH;

        CHAR_INFO * top = new CHAR_INFO[SCORE_CARD_WIDTH * HORIZONTAL_BORDER];
        CHAR_INFO * left = new CHAR_INFO[SCORE_CARD_HEIGHT * VERTICAL_BORDER];
        SetConsoleActiveScreenBuffer(defaultScreenBuffer);

        do {
            // Set color of frame to sparkle or black depending if game was successfull
            for (int x = 0; x < SCORE_CARD_WIDTH; x++) {
                    top[x].Attributes = (sparkle ? (rand() % 0xEE) + 10 : 0x01);
                    top[x].Char.UnicodeChar = L' ';
            }

            for (int x = 0; x < (SCORE_CARD_HEIGHT * 2); x++) {
                    left[x].Attributes = (sparkle ? (rand() % 0xEE) + 10 : 0x01);
                    left[x].Char.UnicodeChar = L' ';
            }

            // Write all score-card frame sides to the console
            succes = WriteConsoleOutputW(
                defaultScreenBuffer,
                top,
                horizontal_buffer_size,
                topLeftCoordinate,
                (&topRect)
            );
            succes = WriteConsoleOutputW(
                defaultScreenBuffer,
                top,
                horizontal_buffer_size,
                topLeftCoordinate,
                (&bottomRect)
            );
            succes = WriteConsoleOutputW(
                defaultScreenBuffer,
                left,
                vertical_buffer_size,
                topLeftCoordinate,
                (&leftRect)
            );
            succes = WriteConsoleOutputW(
                defaultScreenBuffer,
                left,
                vertical_buffer_size,
                topLeftCoordinate,
                (&rightRect)
            );

            // Makes sure any remaining RETURNS are discarded
            GetAsyncKeyState(VK_RETURN);
            GetAsyncKeyState(VK_RETURN);

            if (GetAsyncKeyState(VK_RETURN)) {
                delete[] left;
                delete[] top;
                system("CLS"); // Clear screen before leaving, otherwise menu looks funny when exiting game
                return succes;
                break;
            }
            
            Sleep(50); // Without this the colors in sparkle mode look dull
        } while (true);
    }

    void Console::drawScoreCardScore(CHAR_INFO map[], int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER, int SCORE, int MAX_SCORE) {
        // const char * max_score = std::to_string(MAX_SCORE).c_str();
        const char * score = std::to_string(SCORE).c_str();
        const int LEFT_MARGIN = 4; // 1 for border + actual margin
        const int RIGHT_MARGIN = 1; // 1 for border + actual margin
        const int START_POSITION = (NUMBER_OF_COLUMNS * ROW_NUMBER) + LEFT_MARGIN;
        const int END_OF_LINE = (NUMBER_OF_COLUMNS * (ROW_NUMBER + 1)) - RIGHT_MARGIN;

        unsigned int count = 0;
        
        for (int i = START_POSITION; i < END_OF_LINE; i++) {
            if (!(count > sizeof(score)/sizeof(int))) {
                addCharToMap(map, i, score[count], scoreCardAttribute);
                count++;
            } else {
                addCharToMap(map, i, L' ', scoreCardAttribute);
            }
        }
    }
    
    void Console::drawScoreCardPlayerName(CHAR_INFO map[], int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER) {
        const int LEFT_MARGIN = 4;
        const int RIGHT_MARGIN = 1;
        const int START_POSITION = (NUMBER_OF_COLUMNS * ROW_NUMBER) + LEFT_MARGIN;
        const unsigned int END_OF_LINE = (NUMBER_OF_COLUMNS * (ROW_NUMBER + 1)) - RIGHT_MARGIN;
        const char * PLAYER_NAME = userName.c_str();

        unsigned int count = 0;

        for (unsigned int i = START_POSITION; i < END_OF_LINE; i++) {
            if (!(count > sizeof(PLAYER_NAME)/sizeof(char))) {
                addCharToMap(map, i, PLAYER_NAME[count], scoreCardAttribute);
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
        // ╔═══════════════╗
        // ║               ║
        // ╟───────────────╢
        // ╠═══════════════╣
        // ╚═══════════════╝

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