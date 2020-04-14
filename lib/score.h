#pragma once

#include <iostream>
#include <string>
#include <algorithm>

namespace MyAscii {

    class Score {
        public:
            Score();
        
        public:
            void set_score(std::string name, int difficulty, int correct_guesses, int number_of_sets, double elapsed_time);
            std::string get_name(void);
            int get_difficulty(void);
            int get_correct_guesses(void);
            int get_number_of_sets(void);
            double get_elapsed_time(void);

            bool operator < (const Score& str) const
            {
                return (correct_guesses > str.correct_guesses && number_of_sets > str.number_of_sets && elapsed_time > str.elapsed_time);
            }

        private:
            std::string name;
            int difficulty;
            int correct_guesses;
            int number_of_sets;
            double elapsed_time;

    };

};