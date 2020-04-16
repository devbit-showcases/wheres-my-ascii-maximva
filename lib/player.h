#pragma once

#include <string>

namespace MyAscii {
    class Player {
        public:
            Player (void);
            Player (std::string name);
        
        public:
            void set_name(std::string name);
            std::string get_name(void);

        private:
            std::string name = "guest-player";

    };
};