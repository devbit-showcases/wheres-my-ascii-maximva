#include "Player.h"

namespace MyAscii {

    Player::Player(void)
    : Player("guest-player") {
    }

    Player::Player(std::string name) {
        Player::set_name(name);
    }

    void Player::set_name(std::string name) {
        this->name = name;
    }

    std::string Player::get_name(void) {
        return name;
    }
};