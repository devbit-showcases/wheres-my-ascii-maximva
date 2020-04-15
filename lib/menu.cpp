#include "menu.h"

namespace MyAscii {

    Menu::Menu(Console * console) {
        this->console = console;
    }

    int Menu::show(void) {
        unsigned int current_menu_item = 0;

        std::string menu_items[] = {
            "Play a game",
            "Show the high scores",
            "Close application"
        };

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

            // ROSEBUD (The Sims classic cheat code)
            if (GetKeyState(82) & 8000) {
                system("pause>nul");
                if (GetKeyState(79) & 8000) {
                    system("pause>nul");
                    if (GetKeyState(83) & 8000) {
                        system("pause>nul");
                        if (GetKeyState(69) & 8000) {     // number key values top: 48 - 57   Keypad: 96 - 105
                            system("pause>nul");
                            if (GetKeyState(66) & 8000) {     // number key values top: 48 - 57   Keypad: 96 - 105
                                system("pause>nul");
                                if (GetKeyState(85) & 8000) {     // number key values top: 48 - 57   Keypad: 96 - 105
                                    system("pause>nul");
                                    if (GetKeyState(68) & 8000) {     // number key values top: 48 - 57   Keypad: 96 - 105
                                        console->toggleHiddenCharSecret();
                                    }
                                }
                            }
                        }
                    }
                }
            }


        } while (!GetAsyncKeyState(VK_RETURN) && !GetAsyncKeyState(VK_SPACE));

        if (current_menu_item == 0) {
            console->showMenu(menu_items, sizeof(menu_items)/sizeof(std::string), current_menu_item, true);
        }

        return current_menu_item;
    }
}