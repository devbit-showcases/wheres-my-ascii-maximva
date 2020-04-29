#include "scorecard.h"

namespace MyAscii {

    /**
     * Creates an instance of ScoreCard
     */
    ScoreCard::ScoreCard() {

    }


    /**
     * Adds all scores to the provided score vector pointer
     */
    void ScoreCard::get_scoretable(std::vector<Score> * scores) {
        std::ifstream scoreFile;
        scoreFile.open(scoresFilename);

        if (scoreFile.is_open()) {
            std::string name, difficulty, correctGuesses, numberOfSets, elapsedTime;

            while (getline(scoreFile, name, ',')){
                getline(scoreFile, difficulty, ',');
                getline(scoreFile, correctGuesses, ',');
                getline(scoreFile, numberOfSets, ',');
                getline(scoreFile, elapsedTime);

                Score tempScore(name, stoi(difficulty), stoi(correctGuesses), stod(numberOfSets), stod(elapsedTime));
                scores->push_back(tempScore);
            }
            std::sort(scores->begin(), scores->end());
        }
    }


    /**
     * Adds a score to the scores file
     */
    void ScoreCard::save_score(Score * score) {
        std::ofstream scoreFile;
        scoreFile.open(scoresFilename, std::ios::out | std::ios::app);

        if (scoreFile.is_open()) {
            scoreFile  << score->get_name() << ","
                        << score->get_difficulty() << ","
                        << score->get_correct_guesses() << ","
                        << score->get_number_of_sets() << ","
                        << score->get_elapsed_time()
                        << std::endl;
        }
    }

};