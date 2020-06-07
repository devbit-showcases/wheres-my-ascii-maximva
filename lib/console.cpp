#include "Console.h"

namespace MyAscii {

    /**
     * Creates instance of Console with default title
     */
    Console::Console(void)
    : Console("New window") {
    }


    /**
     * Creates instance of Console with provided title
     */
    Console::Console(std::string windowTitle) {
        init_console_window(windowTitle);
        defaultScreenBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
        create_textmode_screenbuffer(&gameScreenBuffer);
        hide_cursor(&gameScreenBuffer);
        title = FileReader::read_plaintext("title.txt");
    }


    /**
     * Sets the title, background color and sets console to full-screen
     */
    void Console::init_console_window(std::string windowTitle) {
        HWND console = GetConsoleWindow();
        this->windowTitle =  windowTitle;
        SetConsoleTitleA(windowTitle.c_str());
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xEC);
        ShowWindow(console, SW_MAXIMIZE); // Go to full-screen
    }


    /**
     * Hides the cursor on the provided screenbuffer 
     */
    void Console::hide_cursor(HANDLE * screenBuffer) {
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo((*screenBuffer), &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo((*screenBuffer), &cursorInfo);
    }


    /**
     * Shows the cursor on the provided screenbuffer 
     */
    void Console::show_cursor(HANDLE * screenBuffer) {
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo((*screenBuffer), &cursorInfo);
        cursorInfo.bVisible = true;
        SetConsoleCursorInfo((*screenBuffer), &cursorInfo);
    }


    /**
     * Creates textmode read/write screenbuffer
     */ 
    void Console::create_textmode_screenbuffer(HANDLE * screenBuffer) { 
        (*screenBuffer) = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL);
    }


    void Console::toggleHiddenCharSecret(void) {
        hidden_char_secret = !hidden_char_secret;
    }

    bool Console::hidden_char_state(void) {
        return hidden_char_secret;
    }


    bool Console::print_title(void) {
        COORD coordinateBufferSize;
        COORD topLeftCoordinate;
        SMALL_RECT titleRect;
        BOOL succes;

        int bufferWidth = get_screenbuffer_width(&defaultScreenBuffer);
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
        CoordSetter::reset(&topLeftCoordinate);

        titleRect.Top = TOP_MARGIN;
        titleRect.Left = TITLE_CARD_START_POSITION;
        titleRect.Bottom = TOP_MARGIN + TITLE_CARD_HEIGHT;
        titleRect.Right = TITLE_CARD_START_POSITION + TITLE_CARD_WIDTH;

        succes = WriteConsoleOutputW(
            defaultScreenBuffer,
            title_card,
            coordinateBufferSize,
            topLeftCoordinate,
            (&titleRect)
        );

        // Print the title to the console
        COORD tileCoord;
        CoordSetter::set(&tileCoord, (TITLE_CARD_START_POSITION + LEFT_MARGIN), TOP_MARGIN + 2);
        SetConsoleCursorPosition(defaultScreenBuffer, tileCoord);

        for (unsigned int i = 0; i < 6; i++) {  // for some reason tile.size() doesn't, reprints title every time...
            std::cout << title[i];
            tileCoord.Y++;
            SetConsoleCursorPosition(defaultScreenBuffer, tileCoord);
        }

        tileCoord.Y += 4;
        SetConsoleCursorPosition(defaultScreenBuffer, tileCoord);
        std::cout << "Use the [UP] and [DOWN] arrow keys to navigate, [ENTER] to select.";
        CoordSetter::reset(&tileCoord);
        SetConsoleCursorPosition(defaultScreenBuffer, tileCoord);
        return succes;
    }


    /**
     * Adds a menu item to the provided CHAR_INFO map
     */
    void Console::add_menu_item_to_map(CHAR_INFO map[], const char * menuItem, bool currentMenuItem) {
        for (int i = 0; i < MENU_ITEM_HEIGHT * MENU_ITEM_WIDTH; i++) {
            if (i > MENU_ITEM_WIDTH + 1 && i < (2 * MENU_ITEM_WIDTH) - 2) {
                int size = sizeof_text(menuItem);

                if (i - MENU_ITEM_WIDTH - 2 < size) {
                    add_char_to_map(map, i, menuItem[i - MENU_ITEM_WIDTH - 2], MENU_ITEM_ATTRIBUTE);
                } else {
                    add_char_to_map(map, i, L' ', MENU_ITEM_ATTRIBUTE);
                }
            } else {
                add_char_to_map(map, i, L' ', MENU_ITEM_ATTRIBUTE);
            }

            if (currentMenuItem) {
                if (i == 0 || i == MENU_ITEM_WIDTH || i == 2 * MENU_ITEM_WIDTH ) {
                    add_char_to_map(map, i, L' ', 0x40);
                }
            }
        }
    }


    bool Console::print_menu(std::string menuItems[], int itemsSize, int currentMenuItem, UserInput * userInfo, bool userInputNeeded) {
        COORD coordinateBufferSize, topLeftCoordinate;
        SMALL_RECT srcWriteRect;
        BOOL succes;
        int bufferWidth = get_screenbuffer_width(&defaultScreenBuffer);
        const int START_POSITION = (bufferWidth - MENU_ITEM_WIDTH) / 2;
        
        hide_cursor(&defaultScreenBuffer);
        if (userInputNeeded) {
            system("CLS");
            show_cursor(&defaultScreenBuffer);
        }
        print_title();
        
        for (int y = 0; y < itemsSize; y++) {
            CHAR_INFO map[MENU_ITEM_SIZE];
            add_menu_item_to_map(map, menuItems[y].c_str(), (y == currentMenuItem));
            
            const int USER_INPUT_SPACER = ((userInputNeeded) && (y != 0) ? 5 : 0);
            CoordSetter::set(&coordinateBufferSize, MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
            CoordSetter::reset(&topLeftCoordinate);

            srcWriteRect.Top = USER_INPUT_SPACER + MENU_TOP_MARGIN + (y * (MENU_ITEM_HEIGHT + 1));
            srcWriteRect.Left = START_POSITION;
            srcWriteRect.Bottom = USER_INPUT_SPACER + MENU_TOP_MARGIN + (y * (MENU_ITEM_HEIGHT + 1)) + MENU_ITEM_HEIGHT;
            srcWriteRect.Right = (START_POSITION + MENU_ITEM_WIDTH);

            succes = WriteConsoleOutputW(
                defaultScreenBuffer,
                map,
                coordinateBufferSize,
                topLeftCoordinate,
                (&srcWriteRect)
            );
        }
        SetConsoleActiveScreenBuffer(defaultScreenBuffer);
        if (userInputNeeded) {
            COORD cursorCoord;
            CoordSetter::set(&cursorCoord, (START_POSITION + 1), (MENU_TOP_MARGIN + MENU_ITEM_HEIGHT + 1));
            (*userInfo).set_player_name(&defaultScreenBuffer, &cursorCoord);
            cursorCoord.Y += 2;
            (*userInfo).set_game_difficulty(&defaultScreenBuffer, &cursorCoord);
            system("CLS");
        }
        return succes;
    }

    bool Console::print_playfield(std::vector<Tile> * tiles, int fieldEdgeSize, int selectedTileX, int selectedTileY, int difficulty) {
        COORD coordinateBufferSize;
        COORD topLeftCoordinate;
        SMALL_RECT playfieldRect;
        BOOL succes;

        const int TILE_WIDTH = difficulty_tile_size[difficulty][0];   // Has to be an uneven number  9 (min), 13, 17 (no exact center with even number)
        const int TILE_HEIGHT = difficulty_tile_size[difficulty][1];  // Has to be an uneven number  5 (min), 7, 9
        const int MAP_SIZE = TILE_WIDTH * TILE_HEIGHT;
        const int TILE_CENTER = (MAP_SIZE - 1) / 2;
        const int VERTICAL_SPACING = 1;
        const int HORIZONTAL_SPACING = 2;
        const int START_POSITION = (MENU_X_START_POSITION - (fieldEdgeSize * TILE_WIDTH) - ((fieldEdgeSize - 1) * 2)) / 2;

        for (int y = 0; y < fieldEdgeSize; y++) {
            for (int x = 0; x < fieldEdgeSize; x++) {
                CHAR_INFO map[MAP_SIZE];
                const int TILE_ARRAY_INDEX = x + (y * fieldEdgeSize);

                // Display playfield on screen
                for (int j = 0; j < MAP_SIZE; j++) {
                    const wchar_t CHAR_TO_GUESS = ((*tiles)[TILE_ARRAY_INDEX].is_flipped() ? (*tiles)[TILE_ARRAY_INDEX].get_flipped_char() : (*tiles)[TILE_ARRAY_INDEX].get_covered_char()); // ascii : hidden
                    const int TILE_FLIPPED_ATTRIBUTE = (*tiles)[TILE_ARRAY_INDEX].get_flipped_attribute();
                    const int TILE_COVERED_ATTRIBUTE = (*tiles)[TILE_ARRAY_INDEX].get_covered_attribute();
                    const int TILE_SHOW_ATTRIBUTE = ((*tiles)[TILE_ARRAY_INDEX].is_flipped() ? TILE_FLIPPED_ATTRIBUTE : TILE_COVERED_ATTRIBUTE); // Flipped : Covered

                    if (x == selectedTileX && y == selectedTileY) {
                        if (j == (TILE_WIDTH + 2)) {
                            map[j].Char.UnicodeChar = L'┌';
                        } else if ((j > TILE_WIDTH + 2 && j < (2 * TILE_WIDTH) - 3) || ((j > MAP_SIZE - (2 * TILE_WIDTH) + 2) && j < (MAP_SIZE - TILE_WIDTH - 3))){
                            map[j].Char.UnicodeChar = L'─';
                        } else if (j == (2 * TILE_WIDTH) - 3) {
                            map[j].Char.UnicodeChar = L'┐';
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
                        map[j].Char.UnicodeChar = (j == TILE_CENTER ? CHAR_TO_GUESS : L' ');
                    }

                    map[j].Attributes = TILE_SHOW_ATTRIBUTE;
                }

                CoordSetter::set(&coordinateBufferSize, TILE_WIDTH, TILE_HEIGHT);
                CoordSetter::reset(&topLeftCoordinate);

                playfieldRect.Top = GAME_TOP_MARGIN + (y * (TILE_HEIGHT + VERTICAL_SPACING));
                playfieldRect.Left = START_POSITION + (x * (TILE_WIDTH + HORIZONTAL_SPACING));
                playfieldRect.Bottom = GAME_TOP_MARGIN + (y * (TILE_HEIGHT + VERTICAL_SPACING)) + TILE_HEIGHT;
                playfieldRect.Right = START_POSITION + (x * (TILE_WIDTH + HORIZONTAL_SPACING)) + TILE_WIDTH;

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

    void Console::showAboutPage(void) {
        system("CLS");
        const int LEFT_PADDING = 10;
        const int TOP_MARGIN = 10;
        const int TOP_PADDING = 5;
        const int FRAME_WIDTH = 130;
        const int FRAME_HEIGHT = 40;
        int bufferWidth = get_screenbuffer_width(&defaultScreenBuffer);
        const int START_POSITION = (bufferWidth - FRAME_WIDTH) / 2;
        std::vector<std::string> about_page = FileReader::read_plaintext("about.txt");

        COORD cursorCoord;
        CoordSetter::set(&cursorCoord, (START_POSITION + LEFT_PADDING), (TOP_MARGIN + TOP_PADDING));

        for(unsigned int i = 0; i < about_page.size(); i++) {
            SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
            std::cout << about_page[i];
            cursorCoord.Y++;
        }

        draw_frame(&defaultScreenBuffer, bufferWidth, FRAME_HEIGHT, FRAME_WIDTH, TOP_MARGIN, true);
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
        int bufferWidth = get_screenbuffer_width(&defaultScreenBuffer);
        const int START_POSITION = (bufferWidth - FRAME_WIDTH) / 2;
        const int NAME_COLUMN_OFFSET = 10;
        const int SCORE_COLUMN_OFFSET = 17;
        const int TIME_COLUMN_OFFSET = 20;
        const int DIFFICULTY_COLUMN_OFFSET = 20;

        COORD cursorCoord;
        CoordSetter::set(&cursorCoord, (START_POSITION + LEFT_PADDING), (TOP_MARGIN + TOP_PADDING));

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

        cursorCoord.X += DIFFICULTY_COLUMN_OFFSET;
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
            std::cout << get_timestring(scores[i].get_elapsed_time());

            cursorCoord.X += DIFFICULTY_COLUMN_OFFSET;
            SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
            std::cout << "(difficulty " << (scores[i].get_difficulty() + 1) << ")" << std::endl;

            cursorCoord.Y++;
        }

        cursorCoord.X = START_POSITION + LEFT_PADDING;
        cursorCoord.Y += 7;
        SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
        std::cout << "Press [ENTER] to go back to the main menu.";

        draw_frame(&defaultScreenBuffer, bufferWidth, FRAME_HEIGHT, FRAME_WIDTH, TOP_MARGIN, true);
    }


    /**
     * Returns a string format: "mm:ss min." from a type double time argument
     */
    std::string Console::get_timestring(double time) {
        int minutes = time / 60;
        int tens_seconds = (time - (minutes * 60)) / 10; // % Doesn't work for some reason
        int hundreds_seconds = (time - (minutes * 60) - (tens_seconds * 10));
        return std::to_string(minutes) + "." + std::to_string(tens_seconds) + std::to_string(hundreds_seconds) + " min."; 
    }

    void Console::print_endgame_screen(int number_of_pairs, int correct_guesses, double elapsedTime, Player * player) {
        int bufferWidth = get_screenbuffer_width(&defaultScreenBuffer);

        const int SCORE_CARD_WIDTH = 70;
        const int SCORE_CARD_HEIGHT = 16;
        const int TOP_MARGIN = 15;
        const int LEFT_PADDING = 10;
        const int TOP_PADDING = 5;
        const int START_POSITION = (bufferWidth - SCORE_CARD_WIDTH) / 2;
        bool sparkle = (correct_guesses == number_of_pairs);

        std::string gameOverText;
        std::string found_plural = (correct_guesses == number_of_pairs ? " You found all " : " You found ");
        std::string out_of_plural = (correct_guesses == number_of_pairs ? "" : " out of " + std::to_string(number_of_pairs));

        if (correct_guesses == number_of_pairs) {
            gameOverText = "nice job!";
        } else if (correct_guesses == 0) {
            gameOverText = "you didn't even try ...";
        } else {
            gameOverText = "try harder next time?";
        }

        // Output game summary to console screen
        COORD cursorCoord;
        cursorCoord.X = START_POSITION + LEFT_PADDING;
        cursorCoord.Y = TOP_MARGIN + TOP_PADDING;

        SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
        std::cout << "Hi " << (*player).get_name() << ",";
        cursorCoord.Y += 1;
        SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
        std::cout << gameOverText << found_plural << std::to_string(correct_guesses) << out_of_plural << " sets!";
        cursorCoord.Y += 1;
        SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
        std::cout << "It took you " << get_timestring(elapsedTime);
        cursorCoord.Y += 2;
        SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
        std::cout << "Press [ENTER] to go back to the main menu.";

        // After outputting the text, draw a box around it
        draw_frame(&defaultScreenBuffer, bufferWidth, SCORE_CARD_HEIGHT, SCORE_CARD_WIDTH, TOP_MARGIN, sparkle);
    }

    /**
     * Why this very elaborate way to print the scorecard to the console during the game?
     * Using the default properties of the standard console screen buffer allows you to easily use std::cout.
     * However these default properties make the game feel very sluggish.
     * The only way i found to easily print text to the console screen during the game
     * and to make the game run smoothly is to build these custom methods to do it.
    */
    bool Console::show_scorecard(int number_of_pairs, int setSize, int correct_guesses, bool stay_in_game, int difficulty, std::string userName) {
        COORD coordinateBufferSize;
        COORD topLeftCoordinate;
        SMALL_RECT srcWriteRect;
        BOOL succes;
        const int NUMBER_OF_ROWS = 27;
        const int NUMBER_OF_COLUMNS = 50;
        const int SCORECARD_SIZE = NUMBER_OF_ROWS * NUMBER_OF_COLUMNS;
        CHAR_INFO map[SCORECARD_SIZE];

        bool uniqueChars = (difficulty < 3);
        int difficultyLevel = (difficulty + 1);

        std::string setsizeText = "Size of a set: " + std::to_string(setSize);
        std::string difficultylevelText = "Difficulty level: " + std::to_string(difficultyLevel);

        // Print the empty scorecard box
        print_scorecard_structure(map, ScoreCardStructure::TOP, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 0);
        for (int i = 1; i < NUMBER_OF_ROWS - 1; i++) {
            print_scorecard_structure(map, ScoreCardStructure::EMPTY_LINE, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, i);
        }
        print_scorecard_structure(map, ScoreCardStructure::BOTTOM, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, NUMBER_OF_ROWS - 1);
        // Print the game info
        const char * GAME_INFO[] = {
            (char *)"single-division",
            (char *)"",
            (char *)("Player: " + userName).c_str(),
            (char *)("Score: " + std::to_string(correct_guesses) + "/" + std::to_string(number_of_pairs)).c_str(),
            (char *)"",
            (char *)difficultylevelText.c_str(),
            (char *)setsizeText.c_str(),
            (uniqueChars ? (char *)"Every set has a unique letter" : (char *)"Multiple sets can have the same letter"),
            (char *)"",
            (char *)"bottom-division",
            (char *)"blank-division",
            (char *)"top-division"
        };
        print_scorecard_text((char *)"GAME INFO", map, NUMBER_OF_COLUMNS, 1, 0xE9);
        print_scorecard_text_array(GAME_INFO, sizeof(GAME_INFO)/sizeof(const char *), map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 2, scoreCardAttribute);

        // Print the controls info
        const char * CONTROL_INFO[] = {
            (char *)"single-division",
            (char *)"",
            (char *)"Use the arrow keys to move around:",
            (char *)"[UP] [DOWN] [LEFT] [RIGHT]",
            (char *)"",
            (char *)"To flip a tile:",
            (char *)"[RETURN] or [SPACE]",
            (char *)"",
            (char *)"To exit the game:",
            (char *)"[ESC]",
            (char *)""
        };
        print_scorecard_text((char *)"CONTROLS", map, NUMBER_OF_COLUMNS, 14, 0xE9);
        print_scorecard_text_array(CONTROL_INFO, sizeof(CONTROL_INFO)/sizeof(const char *), map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 15, scoreCardAttribute);

        CoordSetter::set(&coordinateBufferSize, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS);
        CoordSetter::reset(&topLeftCoordinate);
        set_smallrect_position(&srcWriteRect, GAME_TOP_MARGIN, (GAME_TOP_MARGIN + NUMBER_OF_ROWS), MENU_X_START_POSITION, (MENU_X_START_POSITION + NUMBER_OF_COLUMNS));

        succes = WriteConsoleOutputW(
            gameScreenBuffer,
            map,
            coordinateBufferSize,
            topLeftCoordinate,
            (&srcWriteRect)
        );

        return succes;
    }

    
    /**
     * Sets the top, bottom, left and right coordinate of the box to print to
     */
    void Console::set_smallrect_position(SMALL_RECT * canvas, int top, int bottom, int left, int right) {
        (*canvas).Top = top;
        (*canvas).Bottom = bottom;
        (*canvas).Left = left;
        (*canvas).Right = right;
    }


    /**
     * Draw frame in black or sparkle mode around anything on the provided screenBuffer
     */
    bool Console::draw_frame(HANDLE * screenBuffer, int bufferWidth, int frameHeight, int frameWidth, int topMargin, bool sparkle) {
        const int START_POSITION = (bufferWidth - frameWidth) / 2;
        const int HORIZONTAL_BORDER = 1;
        const int VERTICAL_BORDER = 2;
        BOOL succes = false;

        COORD topLeftCoordinate;
        CoordSetter::reset(&topLeftCoordinate);
        COORD horizontalBufferSize;
        CoordSetter::set(&horizontalBufferSize, frameWidth, HORIZONTAL_BORDER);
        COORD verticalBufferSize;
        CoordSetter::set(&verticalBufferSize, VERTICAL_BORDER, frameHeight);
        
        // Set position of all frame edges on screen
        SMALL_RECT topRect;
        set_smallrect_position(&topRect, topMargin, topMargin, START_POSITION, (START_POSITION + frameWidth));
        SMALL_RECT bottomRect;
        set_smallrect_position(&bottomRect, (topMargin + frameHeight - 1), (topMargin + frameHeight - 1), START_POSITION, (START_POSITION + frameWidth));
        SMALL_RECT leftRect;
        set_smallrect_position(&leftRect, topMargin, (topMargin + frameHeight), START_POSITION, (START_POSITION + 1));
        SMALL_RECT rightRect;
        set_smallrect_position(&rightRect, topMargin, (topMargin + frameHeight), (START_POSITION + frameWidth - 1), (START_POSITION + frameWidth));

        CHAR_INFO * horizontalMap = new CHAR_INFO[frameWidth * HORIZONTAL_BORDER];
        CHAR_INFO * verticalMap = new CHAR_INFO[frameHeight * VERTICAL_BORDER];
        SetConsoleActiveScreenBuffer(defaultScreenBuffer);

        do {
            // Set color of frame to sparkle or black depending if game was successfull
            for (int x = 0; x < frameWidth; x++) {
                int attribute = (sparkle ? (rand() % 0xEE) + 10 : 0x01);
                add_char_to_map(horizontalMap, x, L' ', attribute);
            }
            for (int x = 0; x < (frameHeight * 2); x++) {
                int attribute = (sparkle ? (rand() % 0xEE) + 10 : 0x01);
                add_char_to_map(verticalMap, x, L' ', attribute);
            }

            // Write all score-card frame sides to the console
            succes = WriteConsoleOutputW(
                defaultScreenBuffer,
                horizontalMap,
                horizontalBufferSize,
                topLeftCoordinate,
                (&topRect)
            );
            succes = WriteConsoleOutputW(
                defaultScreenBuffer,
                horizontalMap,
                horizontalBufferSize,
                topLeftCoordinate,
                (&bottomRect)
            );
            succes = WriteConsoleOutputW(
                defaultScreenBuffer,
                verticalMap,
                verticalBufferSize,
                topLeftCoordinate,
                (&leftRect)
            );
            succes = WriteConsoleOutputW(
                defaultScreenBuffer,
                verticalMap,
                verticalBufferSize,
                topLeftCoordinate,
                (&rightRect)
            );

            // Makes sure any remaining RETURNS are discarded
            GetAsyncKeyState(VK_RETURN);
            GetAsyncKeyState(VK_RETURN);

            Sleep(50); // Slow down the sparkling a little

            if (GetAsyncKeyState(VK_RETURN)) {
                delete[] verticalMap;
                delete[] horizontalMap;
                system("CLS"); // Clear screen before leaving, otherwise menu looks funny when exiting game
                return succes;
                break;
            }
            
        } while (true);
    }


    /**
     * Prints the provided text on the provided line in the in-game scorecard
     */
    void Console::print_scorecard_text(const char * TEXT, CHAR_INFO map[], int NUMBER_OF_COLUMNS, int ROW_NUMBER, int text_attribute) {
        const int START_POSITION = (NUMBER_OF_COLUMNS * ROW_NUMBER) + SCORECARD_LEFT_MARGIN;
        const unsigned int END_OF_LINE = (NUMBER_OF_COLUMNS * (ROW_NUMBER + 1)) - SCORECARD_RIGHT_MARGIN;
        unsigned int textSize = sizeof_text(TEXT);
        
        // Print the text
        for (unsigned int i = START_POSITION; i < END_OF_LINE; i++) {
            if (!(i - START_POSITION > textSize)) {
                add_char_to_map(map, i, TEXT[i - START_POSITION], text_attribute);
            } else {
                add_char_to_map(map, i, L' ', scoreCardAttribute);
            }
        }
    }


    /**
     * Prints the provided char* array starting at the provided line to the in-game scorecard
     */
    void Console::print_scorecard_text_array(const char * TEXT[], int size, CHAR_INFO map[], int numberOfColumns, int numberOfRows, int startingRow, int textAttribute) {
        int row = startingRow;
        for (int i = 0; i < size; i++) {
            if (TEXT[i] == (char *)"single-division") {
                print_scorecard_structure(map, ScoreCardStructure::SINGLE_DIVISION, numberOfColumns, numberOfRows, row);
            } else if (TEXT[i] == (char *)"top-division") {
                print_scorecard_structure(map, ScoreCardStructure::TOP, numberOfColumns, numberOfRows, row);
            } else if (TEXT[i] == (char *)"bottom-division") {
                print_scorecard_structure(map, ScoreCardStructure::BOTTOM, numberOfColumns, numberOfRows, row);
            } else if (TEXT[i] == (char *)"blank-division") {
                print_scorecard_structure(map, ScoreCardStructure::BLANK_LINE, numberOfColumns, numberOfRows, row);
            } else {
                print_scorecard_text(TEXT[i], map, numberOfColumns, row, textAttribute);
            }
            row++;
        }
    }


    /**
     * Prints the structure of the in-game scorecard
     */
    void Console::print_scorecard_structure(CHAR_INFO map[], ScoreCardStructure type, int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER) {
        int typeIndex = (int) type;
        int leftChar = 0;
        int fillChar = 1;
        int rightChar = 2;

        for (int y = 0; y < NUMBER_OF_ROWS; y++) {
            for (int x = 0; x < NUMBER_OF_COLUMNS; x++) {
                const int MAP_POSITION = x + (y * NUMBER_OF_COLUMNS);
                // Print first character of the row
                if (MAP_POSITION == NUMBER_OF_COLUMNS * ROW_NUMBER) {
                    add_char_to_map(
                        map,
                        MAP_POSITION,
                        scorecard_structure_chars[typeIndex][leftChar],
                        scoreCardAttribute
                    );
                // Print all fill characters in between
                } else if (MAP_POSITION > NUMBER_OF_COLUMNS * ROW_NUMBER && MAP_POSITION < (NUMBER_OF_COLUMNS * ROW_NUMBER) + (NUMBER_OF_COLUMNS - 1)) {
                    add_char_to_map(
                        map,
                        MAP_POSITION,
                        scorecard_structure_chars[typeIndex][fillChar],
                        scoreCardAttribute
                    );
                // And print the last character of the row
                } else if (MAP_POSITION == (NUMBER_OF_COLUMNS * ROW_NUMBER) + (NUMBER_OF_COLUMNS - 1)) {
                    add_char_to_map(
                        map,
                        MAP_POSITION,
                        scorecard_structure_chars[typeIndex][rightChar],
                        scoreCardAttribute
                    );
                }
            }
        }
    }


    /**
     * Returns the size of a const char *
     */
    int Console::sizeof_text(const char * TEXT) {
        int textSize = 0;
        while (TEXT[textSize] != '\0') textSize++;
        return textSize;
    }


    /**
     * Adds a char to a specified position on a CHAR_INFO map
     */
    void Console::add_char_to_map(CHAR_INFO map[], int position, wchar_t character, int attribute) {
        map[position].Char.UnicodeChar = character;
        map[position].Attributes = attribute;
    }


    /**
     * Returns the width of the provided screenbuffer
     */
    int Console::get_screenbuffer_width(HANDLE * screenbuffer) {
        CONSOLE_SCREEN_BUFFER_INFO defaultBufferInfo;
        GetConsoleScreenBufferInfo((*screenbuffer), &defaultBufferInfo);
        return defaultBufferInfo.dwSize.X;
    }

}