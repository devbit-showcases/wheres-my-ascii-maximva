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

    Console console("Where's my ASCII");

    Player player;
    Game game(&player, 0);

    game.start();

    return 0;
}