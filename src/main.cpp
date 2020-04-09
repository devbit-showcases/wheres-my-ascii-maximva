#include <iostream>
#include "time.h"
#include <windows.h>
#include <stdio.h>
#include <thread>
#include "../lib/tile.h"
#include "../lib/playfield.h"
#include "../lib/game.h"
#include "../lib/player.h"
#include "../lib/Console.h"

using namespace MyAscii;

int main(void) {
    srand(time(NULL));

    Player player;
    Game game(&player);

    game.start(4);

    return 0;
}