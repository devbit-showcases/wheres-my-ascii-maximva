#include "Console.h"

namespace MyAscii {

    Console::Console(void)
    : Console("New window") {
    }

    Console::Console(std::string windowTitle) {
        init_console_window(windowTitle);
        defaultScreenBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
        create_generic_screenbuffer(&gameScreenBuffer);
        hide_cursor(&gameScreenBuffer);
        title = read_textfile("title.txt");
    }

    void Console::init_console_window(std::string windowTitle) {
        // Set title, background color and go full-screen
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
        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo((*screenBuffer), &cursorInfo);
        cursorInfo.bVisible = false; // set the cursor visibility
        SetConsoleCursorInfo((*screenBuffer), &cursorInfo);
    }


    /**
     * Creates generic read/write screenbuffer
     */ 
    void Console::create_generic_screenbuffer(HANDLE * screenBuffer) { 
        (*screenBuffer) = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL);
    }


    /**
     * Returns vector with lines from provided textfile as elements
     */
    std::vector<std::string> Console::read_textfile(std::string filename) {
        std::vector<std::string> text;
        std::ifstream text_file;
        text_file.open(filename);
        if (text_file.is_open()) {
            std::string part_text;
            while (getline(text_file, part_text)){
                text.push_back(part_text);
            }
        }
        return text;
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


    bool Console::showTitle(void) {
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
        COORD tileCoord;
        tileCoord.X = TITLE_CARD_START_POSITION + LEFT_MARGIN;
        tileCoord.Y = TOP_MARGIN + 2;
        SetConsoleCursorPosition(defaultScreenBuffer, tileCoord);

        for (unsigned int i = 0; i < 6; i++) {  // for some reason tile.size() doesn't, reprints title every time...
            std::cout << title[i];
            tileCoord.Y++;
            SetConsoleCursorPosition(defaultScreenBuffer, tileCoord);
        }

        tileCoord.Y += 4;
        SetConsoleCursorPosition(defaultScreenBuffer, tileCoord);
        std::cout << "Use the [UP] and [DOWN] arrow keys to navigate, [ENTER] to select.";

        // Reset cursor position
        reset_coord(&tileCoord);
        SetConsoleCursorPosition(defaultScreenBuffer, tileCoord);

        return succes;
    }

    bool Console::showMenu(std::string items[], int items_size, int current_menu_item, bool user_input_needed) {
        if (user_input_needed) {
            system("CLS");
        }
        showTitle();

        COORD coordinateBufferSize;
        COORD topLeftCoordinate;
        SMALL_RECT srcWriteRect;
        BOOL succes;

        int bufferWidth = get_screenbuffer_width(&defaultScreenBuffer);
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

            set_coord(&coordinateBufferSize, MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
            reset_coord(&topLeftCoordinate);

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

                set_coord(&coordinateBufferSize, TILE_WIDTH, TILE_HEIGHT);
                reset_coord(&topLeftCoordinate);

                (&playfieldRect)->Top = GAME_TOP_MARGIN + (y * (TILE_HEIGHT + VERTICAL_SPACING));
                (&playfieldRect)->Left = START_POSITION + (x * (TILE_WIDTH + HORIZONTAL_SPACING));
                (&playfieldRect)->Bottom = GAME_TOP_MARGIN + (y * (TILE_HEIGHT + VERTICAL_SPACING)) + TILE_HEIGHT;
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

    void Console::showAboutPage(void) {
        system("CLS");
        const int LEFT_PADDING = 10;
        const int TOP_MARGIN = 10;
        const int TOP_PADDING = 5;
        const int FRAME_WIDTH = 130;
        const int FRAME_HEIGHT = 40;
        int bufferWidth = get_screenbuffer_width(&defaultScreenBuffer);
        const int START_POSITION = (bufferWidth - FRAME_WIDTH) / 2;

        std::vector<std::string> about_page = read_textfile("about.txt");

        COORD cursorCoord;
        cursorCoord.Y = TOP_MARGIN + TOP_PADDING;
        cursorCoord.X = START_POSITION + LEFT_PADDING;

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
        const int MINUTE_MARK_OFFSET = 5;
        const int DIFFICULTY_COLUMN_OFFSET = 20;

        COORD cursorCoord;
        set_coord(&cursorCoord, (START_POSITION + LEFT_PADDING), (TOP_MARGIN + TOP_PADDING));

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

        draw_frame(&defaultScreenBuffer, bufferWidth, FRAME_HEIGHT, FRAME_WIDTH, TOP_MARGIN, true);
    }

    void Console::print_endgame_screen(int number_of_pairs, int correct_guesses) {
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
        std::cout << "Hi " << userName << ",";
        cursorCoord.Y += 1;
        SetConsoleCursorPosition(defaultScreenBuffer, cursorCoord);
        std::cout << gameOverText << found_plural << std::to_string(correct_guesses) << out_of_plural << " sets!";
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
    bool Console::showScoreCard(int number_of_pairs, int correct_guesses, bool stay_in_game) {
        COORD coordinateBufferSize;
        COORD topLeftCoordinate;
        SMALL_RECT srcWriteRect;
        BOOL succes;

        const int PLAYER_NAME_ROW_NUMBER = 4;
        const int SCORE_ROW_NUMBER = 5;
        const int NUMBER_OF_ROWS = 23;
        const int NUMBER_OF_COLUMNS = 50;
        const int SCORECARD_SIZE = NUMBER_OF_ROWS * NUMBER_OF_COLUMNS;
        CHAR_INFO map[SCORECARD_SIZE];

        // Print the empty scorecard box
        print_scorecard_structure(map, ScoreCardStructure::TOP, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 0);
        for (int i = 1; i < NUMBER_OF_ROWS - 1; i++) {
            print_scorecard_structure(map, ScoreCardStructure::EMPTY_LINE, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, i);
        }
        print_scorecard_structure(map, ScoreCardStructure::BOTTOM, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, NUMBER_OF_ROWS - 1);

        // Print the game info
        print_scorecard_text((char *)"GAME INFO", map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 1, 0xE9);
        print_scorecard_structure(map, ScoreCardStructure::SINGLE_DIVISION, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 2);
        const char * PLAYER_NAME_TEXT = ("Player: " + userName).c_str();
        print_scorecard_text(PLAYER_NAME_TEXT, map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, PLAYER_NAME_ROW_NUMBER, scoreCardAttribute); // Print player name
        const char * SCORE_TEXT = ("Score: " + std::to_string(correct_guesses) + "/" + std::to_string(number_of_pairs)).c_str();
        print_scorecard_text(SCORE_TEXT, map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, SCORE_ROW_NUMBER, scoreCardAttribute); // Print score
        print_scorecard_structure(map, ScoreCardStructure::BOTTOM, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 7);
        print_scorecard_structure(map, ScoreCardStructure::BLANK_LINE, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 8);

        // Print the controls info
        print_scorecard_structure(map, ScoreCardStructure::TOP, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 9);
        print_scorecard_text((char *)"CONTROLS", map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 10, 0xE9);
        print_scorecard_structure(map, ScoreCardStructure::SINGLE_DIVISION, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 11);
        print_scorecard_text((char *)"Use the arrow keys to move around:", map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 13, scoreCardAttribute);
        print_scorecard_text((char *)"[UP] [DOWN] [LEFT] [RIGHT]", map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 14, scoreCardAttribute);
        print_scorecard_text((char *)"To flip a tile:", map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 16, scoreCardAttribute);
        print_scorecard_text((char *)"[RETURN] or [SPACE]", map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 17, scoreCardAttribute);
        print_scorecard_text((char *)"To exit the game:", map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 19, scoreCardAttribute);
        print_scorecard_text((char *)"[ESC]", map, NUMBER_OF_COLUMNS, NUMBER_OF_ROWS, 20, scoreCardAttribute);

        // Get ready to write it to the console screenBuffer
        coordinateBufferSize.Y = NUMBER_OF_ROWS;
        coordinateBufferSize.X = NUMBER_OF_COLUMNS;
        reset_coord(&topLeftCoordinate);
        (&srcWriteRect)->Top = GAME_TOP_MARGIN;
        (&srcWriteRect)->Left = MENU_X_START_POSITION;
        (&srcWriteRect)->Bottom = GAME_TOP_MARGIN + NUMBER_OF_ROWS;
        (&srcWriteRect)->Right = MENU_X_START_POSITION + NUMBER_OF_COLUMNS;

        // Write it to the console screenBuffer
        succes = WriteConsoleOutputW(
            gameScreenBuffer,
            map,
            coordinateBufferSize,
            topLeftCoordinate,
            (&srcWriteRect)
        );

        return succes;
    }

    bool Console::draw_frame(HANDLE * screenBuffer, int bufferWidth, int frameHeight, int frameWidth, int topMargin, bool sparkle) {
        const int START_POSITION = (bufferWidth - frameWidth) / 2;
        const int HORIZONTAL_BORDER = 1;
        const int VERTICAL_BORDER = 2;
        BOOL succes;
        
        COORD topLeftCoordinate;
        reset_coord(&topLeftCoordinate);
        COORD horizontalBufferSize;
        set_coord(&horizontalBufferSize, frameWidth, HORIZONTAL_BORDER);
        COORD verticalBufferSize;
        set_coord(&verticalBufferSize, VERTICAL_BORDER, frameHeight);
        
        // Set position of all frame edges on screen
        SMALL_RECT topRect;
        (&topRect)->Top = topMargin;
        (&topRect)->Left = START_POSITION;
        (&topRect)->Bottom = topMargin;
        (&topRect)->Right = START_POSITION + frameWidth;
        SMALL_RECT bottomRect;
        (&bottomRect)->Top = topMargin + frameHeight - 1;
        (&bottomRect)->Left = START_POSITION;
        (&bottomRect)->Bottom = topMargin + frameHeight - 1;
        (&bottomRect)->Right = START_POSITION + frameWidth;
        SMALL_RECT leftRect;
        (&leftRect)->Top = topMargin;
        (&leftRect)->Left = START_POSITION;
        (&leftRect)->Bottom = topMargin + frameHeight;
        (&leftRect)->Right = START_POSITION + 1;
        SMALL_RECT rightRect;
        (&rightRect)->Top = topMargin;
        (&rightRect)->Left = START_POSITION + frameWidth - 1;
        (&rightRect)->Bottom = topMargin + frameHeight;
        (&rightRect)->Right = START_POSITION + frameWidth;

        CHAR_INFO * top = new CHAR_INFO[frameWidth * HORIZONTAL_BORDER];
        CHAR_INFO * left = new CHAR_INFO[frameHeight * VERTICAL_BORDER];
        SetConsoleActiveScreenBuffer(defaultScreenBuffer);

        do {
            // Set color of frame to sparkle or black depending if game was successfull
            for (int x = 0; x < frameWidth; x++) {
                    top[x].Attributes = (sparkle ? (rand() % 0xEE) + 10 : 0x01);
                    top[x].Char.UnicodeChar = L' ';
            }

            for (int x = 0; x < (frameHeight * 2); x++) {
                    left[x].Attributes = (sparkle ? (rand() % 0xEE) + 10 : 0x01);
                    left[x].Char.UnicodeChar = L' ';
            }

            // Write all score-card frame sides to the console
            succes = WriteConsoleOutputW(
                defaultScreenBuffer,
                top,
                horizontalBufferSize,
                topLeftCoordinate,
                (&topRect)
            );
            succes = WriteConsoleOutputW(
                defaultScreenBuffer,
                top,
                horizontalBufferSize,
                topLeftCoordinate,
                (&bottomRect)
            );
            succes = WriteConsoleOutputW(
                defaultScreenBuffer,
                left,
                verticalBufferSize,
                topLeftCoordinate,
                (&leftRect)
            );
            succes = WriteConsoleOutputW(
                defaultScreenBuffer,
                left,
                verticalBufferSize,
                topLeftCoordinate,
                (&rightRect)
            );

            // Makes sure any remaining RETURNS are discarded
            GetAsyncKeyState(VK_RETURN);
            GetAsyncKeyState(VK_RETURN);

            Sleep(50); // Without this the colors in sparkle mode look dull

            if (GetAsyncKeyState(VK_RETURN)) {
                delete[] left;
                delete[] top;
                system("CLS"); // Clear screen before leaving, otherwise menu looks funny when exiting game
                return succes;
                break;
            }
            
        } while (true);
    }

    void Console::print_scorecard_text(const char * TEXT, CHAR_INFO map[], int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER, int text_attribute) {
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
     * Returns the size of a const char *
     */
    int Console::sizeof_text(const char * TEXT) {
        int textSize = 0;
        while (TEXT[textSize] != '\0') textSize++;
        return textSize;
    }

    void Console::print_scorecard_structure(CHAR_INFO map[], ScoreCardStructure type, int NUMBER_OF_COLUMNS, int NUMBER_OF_ROWS, int ROW_NUMBER) {
        int type_index = (int) type;
        int left_char = 0;
        int fill_char = 1;
        int right_char = 2;

        for (int y = 0; y < NUMBER_OF_ROWS; y++) {
            for (int x = 0; x < NUMBER_OF_COLUMNS; x++) {
                const int MAP_POSITION = x + (y * NUMBER_OF_COLUMNS);
                // Print first character of the row
                if (MAP_POSITION == NUMBER_OF_COLUMNS * ROW_NUMBER) {
                    add_char_to_map(
                        map,
                        MAP_POSITION,
                        scorecard_structure_chars[type_index][left_char],
                        scoreCardAttribute
                    );
                // Print all fill characters in between
                } else if (MAP_POSITION > NUMBER_OF_COLUMNS * ROW_NUMBER && MAP_POSITION < (NUMBER_OF_COLUMNS * ROW_NUMBER) + (NUMBER_OF_COLUMNS - 1)) {
                    add_char_to_map(
                        map,
                        MAP_POSITION,
                        scorecard_structure_chars[type_index][fill_char],
                        scoreCardAttribute
                    );
                // And print the last character of the row
                } else if (MAP_POSITION == (NUMBER_OF_COLUMNS * ROW_NUMBER) + (NUMBER_OF_COLUMNS - 1)) {
                    add_char_to_map(
                        map,
                        MAP_POSITION,
                        scorecard_structure_chars[type_index][right_char],
                        scoreCardAttribute
                    );
                }
            }
        }
    }

    /**
     * Adds a char to a specified position on a CHAR_INFO map
     */
    void Console::add_char_to_map(CHAR_INFO map[], int position, wchar_t character, int attribute) {
        map[position].Char.UnicodeChar = character;
        map[position].Attributes = attribute;
    }

    /**
     * Resets x and y coordinate values of the provided COORD object
     */
    void Console::reset_coord(COORD * coord) {
        set_coord(coord, 0, 0);
    }

    /**
     * Sets x and y coordinate values of the provided COORD object with provided values
     */
    void Console::set_coord(COORD * coord, int xPosition, int yPosition) {
        (*coord).X = xPosition;
        (*coord).Y = yPosition;
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