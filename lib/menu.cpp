#include "menu.h"

namespace MyAscii {

    /**
     * Create instance of Menu
     */
    Menu::Menu(Console * console, std::string * menuItems, int menuItemsSize) {
        this->console = console;
        this->menuItems = menuItems;
        this->menuItemsSize = menuItemsSize;
    }


    /**
     * Show the menu
     */
    Screen Menu::show(void) {
        int currentMenuItem = 0;
        console->print_menu(menuItems, menuItemsSize, currentMenuItem, false);

        do {
            system("pause>nul");    // pause after keystroke
            if (GetAsyncKeyState(VK_UP) && currentMenuItem != 0) {
                currentMenuItem--;
                console->print_menu(menuItems, menuItemsSize, currentMenuItem, false);
            } else if (GetAsyncKeyState(VK_DOWN) && currentMenuItem < (menuItemsSize - 1)) {
                currentMenuItem++;
                console->print_menu(menuItems, menuItemsSize, currentMenuItem, false);
            }
            rosebud_easteregg();
        } while (!GetAsyncKeyState(VK_RETURN) && !GetAsyncKeyState(VK_SPACE));

        if (currentMenuItem == 0) {
            console->print_menu(menuItems, menuItemsSize, currentMenuItem, true);
        }

        return (Screen)currentMenuItem;
        // return currentMenuItem;
    }


    /**
     * Check if the player types "ROSEBUD" and toggle the secret charecter if so
     */
    void Menu::rosebud_easteregg() {
        // number key values top: 48 - 57   Keypad: 96 - 105
        if (GetKeyState(82) & 8000) {
            system("pause>nul");
            if (GetKeyState(79) & 8000) {
                system("pause>nul");
                if (GetKeyState(83) & 8000) {
                    system("pause>nul");
                    if (GetKeyState(69) & 8000) {
                        system("pause>nul");
                        if (GetKeyState(66) & 8000) {
                            system("pause>nul");
                            if (GetKeyState(85) & 8000) {
                                system("pause>nul");
                                if (GetKeyState(68) & 8000) {
                                    console->toggleHiddenCharSecret();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}