#pragma once

#include "windows.h"

namespace MyAscii {
    class CoordSetter {
        public:
            void static set_x(COORD * coord, int position);
            void static set_y(COORD * coord, int position);
            void static set(COORD * coord, int xPosition, int yPosition);
            void static reset(COORD * coord);

    };
}