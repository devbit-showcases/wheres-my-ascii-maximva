#pragma once

#include <string>
#include <windows.h>
#include <iostream>
#include "playfield.h"
#include "tile.h"

// To try and display UNICODE chars
#include <io.h>
#include <fcntl.h>

namespace MyAscii {

    class Console {
        public:
            Console(void);
            Console(std::string windowTitle);

        public:
            void showPlayField(std::vector<Tile> * tiles, int fieldEdgeSize, int selectedTileX, int selectedTileY);

        private:
            std::string windowTitle = "New window";

    };
};