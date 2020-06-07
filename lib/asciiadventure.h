#pragma once

#include <iostream>
#include "time.h"
#include <stdio.h>
#include <thread>
#include "player.h"
#include "console.h"
#include "menu.h"
#include <string>
#include "screen.h"
#include "userinput.h"
#include "game.h"
#include "sound.h"

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