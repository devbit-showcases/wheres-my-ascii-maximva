#include "game.h"

namespace MyAscii {

    Game::Game(Player * player, Console * console) {
        this->player = player;
        this->console = console;
    }

    Score Game::start(unsigned int difficulty) {
        Score score; // Doesn't do anything right now
        setDifficulty(difficulty);
        PlayField playfield(fieldEdgeSize, pair_size, difficulty);
        std::vector<Tile> tiles = playfield.getPlayField();
 
        bool correct_guess = true;
        bool stay_in_game = true;
        std::vector<int> guess_possitions;
        int guessId = 0;
        int number_of_pairs = tiles.size() / pair_size;
        int correct_guesses = 0;
        int selectedTileX = 0;
        int selectedTileY = 0;
        int cards_turned = 0;

        console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
        console->showScoreCard(number_of_pairs, correct_guesses, stay_in_game);

        do {
            // Flipping back wrong guess and resetting
            if (!correct_guess) {
                Sleep(600);     // Or system("pause>nul");  ... don't now what feels better...
                for (int i = 0; i < guess_possitions.size(); i++) {
                    tiles[guess_possitions[i]].turnCard();
                }
                console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
                // Reset for next try
                guess_possitions.clear();
                cards_turned = 0;
                guessId = 0;
                correct_guess = true;
            }

            // Clear any remaining returns or spaces
            GetAsyncKeyState(VK_RETURN);
            GetAsyncKeyState(VK_SPACE);
            
            // Arrow keys to select tiles and RETURN/SPACE to select
            do {
                system("pause>nul");    // pause after keystroke
                if (GetAsyncKeyState(VK_UP) && selectedTileY != 0) {
                    selectedTileY--;
                    console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
                } else if (GetAsyncKeyState(VK_DOWN) && selectedTileY < (fieldEdgeSize - 1)) {
                    selectedTileY++;
                    console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
                } else if (GetAsyncKeyState(VK_LEFT) && selectedTileX != 0) {
                    selectedTileX--;
                    console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
                } else if (GetAsyncKeyState(VK_RIGHT) && selectedTileX < (fieldEdgeSize - 1)) {
                    selectedTileX++;
                    console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
                } else if (GetAsyncKeyState(VK_ESCAPE)) {
                    stay_in_game = false; // Escape to main menu with ESC key
                    break;
                }
            } while (!GetAsyncKeyState(VK_RETURN) && !GetAsyncKeyState(VK_SPACE));

            int current_selected_tile = (selectedTileX) + (selectedTileY * playfield.getFieldEdgeSize());

            // Game mechanics
            if (tiles[current_selected_tile].isTurned()) {
                // Do nothing
            } else {
                tiles[current_selected_tile].turnCard();
                cards_turned++;
                if (cards_turned == 1) {
                    guessId = tiles[current_selected_tile].getId();
                    guess_possitions.push_back(current_selected_tile);
                } else if (cards_turned < pair_size && guessId == tiles[current_selected_tile].getId()) {
                    guess_possitions.push_back(current_selected_tile);
                } else if (cards_turned == pair_size && guessId == tiles[current_selected_tile].getId()) {
                    correct_guesses++;
                    console->showScoreCard(number_of_pairs, correct_guesses, stay_in_game);
                    // Reset for next try
                    cards_turned = 0;
                    guessId = 0;
                    guess_possitions.clear();
                } else {
                    guess_possitions.push_back(current_selected_tile);
                    correct_guess = false;
                }
            }

            console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
            if (correct_guesses == number_of_pairs) {
                stay_in_game = false;
                break; // For some reason just setting stay_in_game to be false doesn't work
            }

        } while (stay_in_game);

        // Will show the end-game score card
        console->showScoreCard(number_of_pairs, correct_guesses, stay_in_game);

        return score;
    }

    void Game::setDifficulty(unsigned int difficulty) {
        this->difficulty = difficulty;
        fieldEdgeSize = gameParameters[difficulty][0];
        pair_size = gameParameters[difficulty][1];
    }
}