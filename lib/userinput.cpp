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
        bool is_number = false;
        do {
            SetConsoleCursorPosition((*screenBuffer), (*cursorCoord));
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
    }


    std::string UserInput::get_player_name(void) {
        return playerName;
    }

    int UserInput::get_game_difficulty(void) {
        return difficulty;
    }




}