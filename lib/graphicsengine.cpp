#include "graphicsengine.h"

namespace MyAscii {

    GraphicsEngine::GraphicsEngine(void)
    : GraphicsEngine("New window") {

    }

    GraphicsEngine::GraphicsEngine(std::string windowTitle) {
        this->windowTitle =  windowTitle;
        setConsoleTitle(windowTitle);
    }

    void GraphicsEngine::setConsoleTitle(std::string windowTitle) {
        SetConsoleTitleA(windowTitle.c_str());
    }



}