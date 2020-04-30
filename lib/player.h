#pragma once

#include <string>

namespace MyAscii {
    class Player {
        public:
            Player (void);
            Player (std::string name);
        
        public:
            std::string get_name(void);
            void set_name(std::string name);

        private:
            std::string name = "guest-player";

    };
};