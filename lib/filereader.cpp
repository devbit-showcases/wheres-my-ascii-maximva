#include "filereader.h"

namespace MyAscii {

    std::vector<std::string> FileReader::read_plaintext(std::string filename) {
        std::ifstream text_file = open_file(filename);
        std::vector<std::string> text;
        if (text_file.is_open()) {
            std::string part_text;
            while (getline(text_file, part_text)){
                text.push_back(part_text);
            }
        }
        return text;
    }

    std::ifstream FileReader::open_file(std::string filename) {
        std::ifstream file;
        file.open(filename);
        return file;
    }

    std::ofstream FileReader::open_file_to_append(std::string filename) {
        std::ofstream file;
        file.open(filename, std::ios::out | std::ios::app);
        return file;
    }


}