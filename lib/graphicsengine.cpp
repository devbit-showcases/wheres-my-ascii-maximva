#include "graphicsengine.h"

namespace MyAscii {

    GraphicsEngine::GraphicsEngine(void)
    : GraphicsEngine("New window") {

    }

    GraphicsEngine::GraphicsEngine(std::string windowTitle) {
        this->windowTitle =  windowTitle;
        setConsoleTitle();
    }

    void GraphicsEngine::setConsoleTitle(void) {
        LPCSTR text = windowTitle.c_str();
	    SetConsoleTitleA(text);
    }



}