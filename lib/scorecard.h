#pragma once

#include <string>
#include "score.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include "filereader.h"

namespace MyAscii {
    class ScoreCard {
        public:
            ScoreCard();

        public:
            void save_score(Score * score);
            void get_scoretable(std::vector<Score> * scores);

        private:
            std::vector<std::string> scores;
            std::string scoresFilename = "scorecard.txt";

    };
};