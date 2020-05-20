#include "filereader.h"

namespace MyAscii {

    std::vector<std::string> FileReader::read_plaintext(std::string filename) {
        std::vector<std::string> text;
        std::ifstream text_file;
        text_file.open(filename);
        if (text_file.is_open()) {
            std::string part_text;
            while (getline(text_file, part_text)){
                text.push_back(part_text);
            }
        }
        return text;
    }

}