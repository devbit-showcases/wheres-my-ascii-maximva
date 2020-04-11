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
#include "../lib/menu.h"

using namespace MyAscii;

int main(void) {
    srand(time(NULL));

    Console console("Where's my ASCII");
    Menu menu(&console);
    int chosen_menu_item = menu.show();

    if (chosen_menu_item == 0) {
        std::string userName = console.getUserName();
        int difficulty = console.getDifficulty();

        Player player;
        player.set_name(userName);
        
        Game game(&player, &console);
        game.start(difficulty);
    } else {
        return 0;
    }

    return 0;
}