#include "userinput.h"

namespace MyAscii {

    void UserInput::set_player_name(HANDLE * screenBuffer, COORD * cursorCoord) {
        do {
            SetConsoleCursorPosition((*screenBuffer), (*cursorCoord));
            std::cout << "Who's playing? ";
            std::getline(std::cin, playerName);
        } while (playerName.length() == 0);
    }


    void UserInput::set_game_difficulty(HANDLE * screenBuffer, COORD * cursorCoord) {
        bool validDifficulty = false;
        do {
            SetConsoleCursorPosition((*screenBuffer), (*cursorCoord));
            std::cout << "What can you handle (1 - 4)? ";
            validDifficulty = get_difficulty_value();
        } while (!validDifficulty);
        difficulty--; // Zero indexed for internal use
    }


    bool UserInput::get_difficulty_value(void) {
        bool validValue = false;
        std::cin >> difficulty;
        if (std::cin.good()){
            validValue = (difficulty < 1 || difficulty > 4 ? false : true);
        } else {
            std::cin.clear();
            std::cin.ignore(100000,'\n');
        }
        return validValue;
    }


    std::string UserInput::get_player_name(void) {
        return playerName;
    }


    int UserInput::get_game_difficulty(void) {
        return difficulty;
    }

}