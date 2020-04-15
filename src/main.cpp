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
    // Menu item numbers
    const int PLAY_GAME = 0;
    const int SHOW_SCORES = 1;

    srand(time(NULL));
    do {
        Console console("ASCII Adventure");
        Menu menu(&console);
        int chosen_menu_item = menu.show();

        if (chosen_menu_item == PLAY_GAME) {
            std::string userName = console.getUserName();
            int difficulty = console.getDifficulty();

            Player player;
            player.set_name(userName);

            Game game(&player, &console);
            PlaySound(TEXT("./sound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            Score score = game.start(difficulty);
            PlaySound(NULL, 0, 0); // Stops the music
        } else  if (chosen_menu_item == SHOW_SCORES) {
            console.showScoreTable();
        } else {
            return 0;
        }
    } while (true);

    return 0;
}