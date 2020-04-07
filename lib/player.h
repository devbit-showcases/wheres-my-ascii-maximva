#pragma once

#include <string>

namespace MyAscii {
    class Player {
        public:
            Player (std::string name);
            Player ();
        
        public:
            std::string get_name();
            void set_name(std::string name);

        private:
            std::string name = "guest-player";

    };
};