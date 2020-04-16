#pragma once

#include <iostream>
#include <string>
#include "score.h"
#include <vector>
#include <fstream>
#include <algorithm>

namespace MyAscii {
    class ScoreCard {
        public:
            ScoreCard();

        public:
            void save_score(Score * score);
            void get_scoretable(std::vector<Score> * scores);

        private:
            std::vector<std::string> score_array;

    };
};