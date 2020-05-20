#include "asciiadventure.h"

namespace MyAscii {

    void AsciiAdventure::start(void) {
        srand(time(NULL));
        show_menu();
    }

    void AsciiAdventure::show_menu(void) {
        Screen next;
        do {
            Console console("ASCII Adventure");
            int menuItemsSize = (sizeof(menuItems)/sizeof(std::string));
            UserInput userInfo;
            Menu menu(&console, menuItems, menuItemsSize);
            next = menu.show(&userInfo);

            if (next == Screen::PLAY_GAME) {
                std::string userName = userInfo.get_player_name();
                int difficulty = userInfo.get_game_difficulty();
                Player player;
                player.set_name(userName);
                PlaySound(TEXT("./sound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                Game game(&player, &console, difficulty);
                game.start();
                PlaySound(NULL, 0, 0); // Stops the music
            } else  if (next == Screen::SHOW_SCORES) {
                console.showScoreTable();
            } else if (next == Screen::ABOUT_PAGE) {
                console.showAboutPage();
            } else {
                break;
            }

        } while (true);
    }


}