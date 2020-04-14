#include "scorecard.h"

namespace MyAscii {

    ScoreCard::ScoreCard() {

    }

    void ScoreCard::get_scoretable(std::vector<Score> * scores) {
        std::ifstream score_file;
        score_file.open("scorecard.txt");

        // Print all the scorecard entries
        if (score_file.is_open()) {
            std::string name, difficulty, correct_guesses, number_of_sets, elapsed_time;
            // const int MAX_NAME_SIZE = 14;
            const int MAX_TIME_SIZE = 5;
            // std::vector<Score> score_list_array;

            while (getline(score_file, name, ',')){
                getline(score_file, difficulty, ',');
                getline(score_file, correct_guesses, ',');
                getline(score_file, number_of_sets, ',');
                getline(score_file, elapsed_time);

                Score temp_score;
                temp_score.set_score(name, stoi(difficulty), stoi(correct_guesses), stod(number_of_sets), stod(elapsed_time));
                scores->push_back(temp_score);
            }
            std::sort(scores->begin(), scores->end());
        }
    }

    void ScoreCard::save_score(Score * score) {
        game_score = score;
        std::ofstream score_file;
        score_file.open("scorecard.txt", std::ios::out | std::ios::app);

        if (score_file.is_open()) {
            score_file  << score->get_name() << ","
                        << score->get_difficulty() << ","
                        << score->get_correct_guesses() << ","
                        << score->get_number_of_sets() << ","
                        << score->get_elapsed_time()
                        << std::endl;
        }
    }

};