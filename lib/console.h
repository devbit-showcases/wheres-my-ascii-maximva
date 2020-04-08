#pragma once

#include <string>
#include <windows.h>
#include <iostream>
#include "playfield.h"
#include "tile.h"

namespace MyAscii {

    class Console {
        public:
            Console(void);
            Console(std::string windowTitle);

        public:
            void setConsoleTitle(std::string windowTitle);

            void showPlayField(void);
            // void updatePlayField(void);

            // void showScoreCard(void);
            // void updateScoreCard(void);

            // void showScoreBoard(void);
            // void updateScoreBoard(void);



        private:
            std::string windowTitle = "New window";

    };
};