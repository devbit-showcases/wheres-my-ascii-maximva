#include "score.h"

namespace MyAscii {

    Score::Score() {

    }

    void Score::set_score(std::string name, int difficulty, int correct_guesses, int number_of_sets, double elapsed_time) {
        this->name = name;
        this->difficulty = difficulty;
        this->correct_guesses = correct_guesses;
        this->number_of_sets = number_of_sets;
        this->elapsed_time = elapsed_time;
    }

    std::string Score::get_name(void) {
        return name;
    }

    int Score::get_difficulty(void) {
        return difficulty;
    }

    int Score::get_correct_guesses(void) {
        return correct_guesses;
    }

    int Score::get_number_of_sets(void) {
        return number_of_sets;
    }

    double Score::get_elapsed_time(void){
        return elapsed_time;
    }
}