#pragma once

#include <vector>
#include <string>
#include "fstream"

namespace MyAscii {

    class FileReader {
        public:
            std::vector<std::string> static read_plaintext(std::string filename);
            std::ifstream static open_file(std::string filename);
            std::ofstream static open_file_to_append(std::string filename);
    };

}