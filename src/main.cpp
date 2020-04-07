#include <iostream>
#include "../lib/tile.h"
#include "../lib/playfield.h"
#include "time.h"
#include <windows.h>
#include <stdio.h>
#include <thread>
#include "../lib/game.h"
#include "../lib/player.h"

using namespace MyAscii;

int main(void) {
    
    srand(time(NULL));

    Player player;
    Game game(&player, 2);

    game.start();



    // bool bKey[5];
    // while (true) {
    //     Sleep(70);
    //     for (int i = 0; i < 5; i++) {
    //         bKey[i] = (0x8000 & GetAsyncKeyState((unsigned char)("\x25\x26\x27\x28\x0D"[i]))) != 0;
    //     }

    //     if (bKey[0]) {
    //         std::cout << "left" << std::endl;
    //     } else if (bKey[1]) {
    //         std::cout << "up" << std::endl;
    //     } else if (bKey[2]) {
    //         std::cout << "right" << std::endl;
    //     } else if (bKey[3]) {
    //         std::cout << "down" << std::endl;
    //     } else if (bKey[4]) {
    //         std::cout << "enter" << std::endl;
    //     }

    // }


    return 0;
}