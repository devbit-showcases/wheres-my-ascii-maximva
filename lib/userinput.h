#pragma once

#include <windows.h>
#include <string>
#include <iostream>

namespace MyAscii {

    class UserInput {
        public:
            void set_player_name(HANDLE * screeBuffer, COORD * cursorCoord);
            void set_game_difficulty(HANDLE * screenBuffer, COORD * cursorCoord);
            std::string get_player_name(void);
            int get_game_difficulty(void);
        
        private:
            bool get_difficulty_value(void);

        private:
            std::string playerName = "";
            int difficulty = 0;

    };

}