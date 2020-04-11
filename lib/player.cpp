#include "Player.h"

namespace MyAscii {

    Player::Player(std::string name) {
        Player::set_name(name);
    }

    Player::Player()
    : Player("guest-player") {
    }

    void Player::set_name(std::string name) {
        this->name = name;
    }

    std::string Player::get_name() {
        return name;
    }
};