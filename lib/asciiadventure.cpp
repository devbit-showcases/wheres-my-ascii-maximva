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
            UserInput userInfo;
            int numberOfMenuItems = (sizeof(menuItems)/sizeof(std::string));
            Menu menu(&console, menuItems, numberOfMenuItems);
            next = menu.show(&userInfo);

            if (next == Screen::PLAY_GAME) {
                Player player;
                player.set_name(userInfo.get_player_name());
                Sound::start();
                Game game(&player, &console, userInfo.get_game_difficulty());
                game.start();
                Sound::stop();
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