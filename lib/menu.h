#pragma once

#include "console.h"

namespace MyAscii {

    class Menu {
        public:
            Menu(Console * console);

        public:
            int show(void);

        private:
            Console * console;
    };
};