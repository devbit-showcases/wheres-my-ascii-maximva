#include "menu.h"

namespace MyAscii {

    Menu::Menu(Console * console) {
        this->console = console;
    }

    int Menu::show(void) {

        const int MENU_WIDTH = 100;  // In columns
        const int MENU_HEIGHT = 40;  // In rows

        int current_menu_item = 0;

        std::string menu_items[] = {
            "Play a game",
            "Close application",
        };

        console->showMenu(menu_items, sizeof(menu_items)/sizeof(std::string), current_menu_item);

        do {
            system("pause>nul");    // pause after keystroke
            if (GetAsyncKeyState(VK_UP) && current_menu_item != 0) {
                current_menu_item--;
                console->showMenu(menu_items, sizeof(menu_items)/sizeof(std::string), current_menu_item);
            } else if (GetAsyncKeyState(VK_DOWN) && current_menu_item < (sizeof(menu_items)/sizeof(std::string) - 1)) {
                current_menu_item++;
                console->showMenu(menu_items, sizeof(menu_items)/sizeof(std::string), current_menu_item);
            }
        } while (!GetAsyncKeyState(VK_RETURN) && !GetAsyncKeyState(VK_SPACE));

        return current_menu_item;

    }

}