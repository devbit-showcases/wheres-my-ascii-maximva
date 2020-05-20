#pragma once

#include <iostream>
#include "time.h"
#include <windows.h>
#include <stdio.h>
#include <thread>
#include "tile.h"
#include "playfield.h"
#include "game.h"
#include "player.h"
#include "Console.h"
#include "menu.h"
#include "score.h"
#include <string>
#include "screen.h"
#include "userinput.h"

namespace MyAscii {

    class AsciiAdventure {
        public:
            void start(void);
        
        private:
            void show_menu(void);

        private:
            std::string menuItems[4] = {
                "Play a game",
                "Show the high scores",
                "About ASCII ADVENTURE",
                "Back to reality"
            };

    };

}