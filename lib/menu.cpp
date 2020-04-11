#include "menu.h"

namespace MyAscii {

    Menu::Menu(Console * console) {
        this->console = console;
    }

    int Menu::show(void) {
        int current_menu_item = 0;

        std::string menu_items[] = {
            "Play a game",
            "Close application"        };

        console->showMenu(menu_items, sizeof(menu_items)/sizeof(std::string), current_menu_item, false);

        do {
            system("pause>nul");    // pause after keystroke
            if (GetAsyncKeyState(VK_UP) && current_menu_item != 0) {
                current_menu_item--;
                console->showMenu(menu_items, sizeof(menu_items)/sizeof(std::string), current_menu_item, false);
            } else if (GetAsyncKeyState(VK_DOWN) && current_menu_item < (sizeof(menu_items)/sizeof(std::string) - 1)) {
                current_menu_item++;
                console->showMenu(menu_items, sizeof(menu_items)/sizeof(std::string), current_menu_item, false);
            }
        } while (!GetAsyncKeyState(VK_RETURN) && !GetAsyncKeyState(VK_SPACE));

        if (current_menu_item == 0) {
            console->showMenu(menu_items, sizeof(menu_items)/sizeof(std::string), current_menu_item, true);
        }

        return current_menu_item;
    }
}