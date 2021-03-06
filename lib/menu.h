#pragma once

#include "console.h"
#include "screen.h"
#include "userinput.h"

namespace MyAscii {

    class Menu {
        public:
            Menu(Console * console, std::string * menuItems, int menuItemsSize);

        public:
            Screen show(UserInput * userInfo);
        
        private:
            void rosebud_easteregg();

        private:
            Console * console;
            std::string * menuItems;
            int menuItemsSize = 0;
    };
};