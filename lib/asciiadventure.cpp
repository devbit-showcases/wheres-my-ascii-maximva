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
                Player player;
                player.set_name(userInfo.get_player_name());
                Game game(&player, &console, userInfo.get_game_difficulty());
                PlaySound(TEXT("./sound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
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