#pragma once

#include <string>
#include <windows.h>
#include <iostream>

namespace MyAscii {

    class GraphicsEngine {
        public:
            GraphicsEngine(void);
            GraphicsEngine(std::string title);

        private:
            void setConsoleTitle(void);

        private:
            std::string windowTitle = "New window";

    };
};