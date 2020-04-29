#include "Player.h"

namespace MyAscii {

    /**
     * Create anonymous instance of Player
     */
    Player::Player(void)
    : Player("guest-player") {
    }


    /**
     * Create an instance of Player
     */
    Player::Player(std::string name) {
        set_name(name);
    }


    /**
     * Set the players name
     */
    void Player::set_name(std::string name) {
        this->name = name;
    }


    /**
     * Get the players name
     */
    std::string Player::get_name(void) {
        return name;
    }
};