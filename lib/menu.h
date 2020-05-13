#pragma once

#include "console.h"
#include "screen.h"

namespace MyAscii {

    class Menu {
        public:
            Menu(Console * console, std::string * menuItems, int menuItemsSize);

        public:
            Screen show(void);
        
        private:
            void rosebud_easteregg();

        private:
            Console * console;
            std::string * menuItems;
            int menuItemsSize = 0;
    };
};