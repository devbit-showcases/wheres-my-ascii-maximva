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
#include "../lib/score.h"

using namespace MyAscii;

int main(void) {
    const int PLAY_GAME = 0;
    const int SHOW_SCORES = 1;
    const int ABOUT_PAGE = 2;
    srand(time(NULL));

    do {
        Console console("ASCII Adventure");
        std::string menuItems[] = {
            "Play a game",
            "Show the high scores",
            "About ASCII ADVENTURE",
            "Back to reality"
        };
        int menuItemsSize = (sizeof(menuItems)/sizeof(std::string));
        Menu menu(&console, menuItems, menuItemsSize);
        int chosen_menu_item = menu.show();

        if (chosen_menu_item == PLAY_GAME) {

            std::string userName = console.getUserName();
            int difficulty = console.getDifficulty();
            Player player;
            player.set_name(userName);
            PlaySound(TEXT("./sound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            Game game(&player, &console, difficulty);
            game.start();
            PlaySound(NULL, 0, 0); // Stops the music

        } else  if (chosen_menu_item == SHOW_SCORES) {

            console.showScoreTable();

        } else if (chosen_menu_item == ABOUT_PAGE) {

            console.showAboutPage();

        } else {

            return 0;
        }

    } while (true);
    return 0;
}