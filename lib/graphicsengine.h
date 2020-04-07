#pragma once

#include <string>
#include <windows.h>
#include <iostream>

namespace MyAscii {

    class GraphicsEngine {
        public:
            GraphicsEngine(void);
            GraphicsEngine(std::string windowTitle);

        public:
            void setConsoleTitle(std::string windowTitle);

        private:
            std::string windowTitle = "New window";

    };
};