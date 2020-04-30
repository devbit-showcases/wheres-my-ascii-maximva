#pragma once

#include <iostream>
#include <time.h>

namespace MyAscii {
    class Tile {
        public:
            Tile(unsigned int id, int flipped_attribute, bool secret);
        
        public:
            unsigned int get_id(void);
            void flip_tile(void);
            void set_random_char(void);
            bool is_flipped(void);
            char get_covered_char(void);
            char get_flipped_char(void);
            int get_flipped_attribute(void);
            int get_covered_attribute(void);

        private:
            bool turned = false;
            bool secret = false;
            int flippedAttribute = 0x31;
            int coveredAttribute = 0x20;
            char flippedChar;
            wchar_t coveredChar = L'?';
            wchar_t secretHiddenChar = L'$';
            unsigned int id = 0;
            unsigned int asciiTableStart = 'A';
            unsigned int asciiTableEnd = 'Z';
            unsigned int asciiTableRange = asciiTableEnd - asciiTableStart;
    };
};