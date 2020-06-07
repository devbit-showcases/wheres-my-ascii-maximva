#pragma once

#include <vector>
#include <string>
#include "fstream"

namespace MyAscii {

    class FileReader {
        public:
            std::vector<std::string> static read_plaintext(std::string filename);

    };

}