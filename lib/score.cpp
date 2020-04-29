#include "score.h"

namespace MyAscii {

    /**
     * Creates an instance of Score
     */
    Score::Score(std::string name, int difficulty, int correctGuesses, int numberOfSets, double elapsedTime) {
        this->name = name;
        this->difficulty = difficulty;
        this->correctGuesses = correctGuesses;
        this->numberOfSets = numberOfSets;
        this->elapsedTime = elapsedTime;
    }


    /**
     * Get the games player name stored in the score
     */
    std::string Score::get_name(void) {
        return name;
    }


    /**
     * Get the game difficulty level stored in the score
     */
    int Score::get_difficulty(void) {
        return difficulty;
    }


    /**
     * Get the games amount of correct guesses stored in the score
     */
    int Score::get_correct_guesses(void) {
        return correctGuesses;
    }


    /**
     * Get the amount of sets available in game, stored in the score
     */
    int Score::get_number_of_sets(void) {
        return numberOfSets;
    }


    /**
     * Get the games elapsed time stored in the score
     */
    double Score::get_elapsed_time(void){
        return elapsedTime;
    }
}