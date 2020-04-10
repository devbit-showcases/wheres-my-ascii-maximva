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

    // crashes the menu when using arrows ...

    // std::string player_name = "";
    // std::cout << "Please choose a name: ";
    // std::cin >> player_name;

    // int difficulty = 0;
    // std::cout << "Please select a dificulty between 1 and 5: ";
    // std::cin >> difficulty;
    


    Console console("Where's my ASCII");

    Menu menu(&console);
    int chosen_menu_item = menu.show();


    if (chosen_menu_item == 0) {
        Player player;
        Game game(&player, &console);
        game.start(1);
    } else {
        return 0;
    }


    return 0;
}