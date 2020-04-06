#include <iostream>

namespace MyAscii {

    class Tile {
        public:
            Tile(void);
            char showHiddenChar(void);



        private:
            char asciiChar;
            char hiddenChar = '?';
            const unsigned int ASCII_TABLE_RANGE = 26;
            const unsigned int ASCII_TABLE_START = 65;
            const unsigned int ASCII_TABLE_START = 90;

    };
};

// CAP CASE characters from ascii [65 - 90] 