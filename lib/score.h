#pragma once

#include <iostream>
#include <string>
#include <algorithm>

namespace MyAscii {

    class Score {
        public:
            Score(std::string name, int difficulty, int correctGuesses, int numberOfSets, double elapsedTime);
        
        public:
            std::string get_name(void);
            int get_difficulty(void);
            int get_correct_guesses(void);
            int get_number_of_sets(void);
            double get_elapsed_time(void);

            bool operator < (const Score& str) const
            {
                if( correctGuesses != str.correctGuesses)
                    return (correctGuesses > str.correctGuesses);
                return (elapsedTime < str.elapsedTime);
            }

        private:
            std::string name;
            int difficulty;
            int correctGuesses;
            int numberOfSets;
            double elapsedTime;

    };

};