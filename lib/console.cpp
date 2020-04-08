#include "Console.h"

namespace MyAscii {

    Console::Console(void)
    : Console("New window") {
    }

    Console::Console(std::string windowTitle) {
        this->windowTitle =  windowTitle;
        setConsoleTitle(windowTitle);
    }

    void Console::setConsoleTitle(std::string windowTitle) {
        SetConsoleTitleA(windowTitle.c_str());
    }

    void Console::showPlayField(void) {

    }


}