#include "coordsetter.h"

namespace MyAscii {

    /**
     * Resets x and y coordinate values of the provided COORD object
     */
    void CoordSetter::reset(COORD * coord) {
        set(coord, 0, 0);
    }


    /**
     * Sets x and y coordinate values of the provided COORD object with provided values
     */
    void CoordSetter::set(COORD * coord, int xPosition, int yPosition) {
        set_x(coord, xPosition);
        set_y(coord, yPosition);
    }


    /**
     * Set x coordinate value of provided COORD object
     */
    void CoordSetter::set_x(COORD * coord, int position) {
        (*coord).X = position;
    }


    /**
     * Set y coordinate value of provided COORD object
     */
     void CoordSetter::set_y(COORD * coord, int position) {
        (*coord).Y = position;
    }

}