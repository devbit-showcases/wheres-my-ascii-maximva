#include "game.h"

namespace MyAscii {

    Game::Game(Player * player, Console * console) {
        this->player = player;
        this->console = console;
    }

    void Game::show_card_cheat(int correct_guesses, int cards_turned, std::vector<Tile> * tiles, int selectedTileX, int selectedTileY) {
        unsigned int cards_to_turn = (fieldEdgeSize * fieldEdgeSize) - (correct_guesses * pair_size) - cards_turned;
        std::vector<int> used_indexes = {};

        for (unsigned int i = 0; (i < cards_to_turn) && (i < 5); i++) {
            int random_index = rand() % (fieldEdgeSize * fieldEdgeSize);

            // Check if tile is turned or has been used in this show of cards allready
            while ((*tiles)[random_index].is_flipped() || std::count(used_indexes.begin(), used_indexes.end(), random_index)) {
                random_index = rand() % (fieldEdgeSize * fieldEdgeSize);
            }
            used_indexes.push_back(random_index);

            // Turn each card for a short period of time
            (*tiles)[random_index].flip_tile();
            console->showPlayField(tiles, fieldEdgeSize, selectedTileX, selectedTileY);
            Sleep(500);
            (*tiles)[random_index].flip_tile();
            console->showPlayField(tiles, fieldEdgeSize, selectedTileX, selectedTileY);
        }
    }

    Score Game::start(unsigned int difficulty) {
        setDifficulty(difficulty);
        bool hidden_char_secret = console->hiddenCharState();
        PlayField playfield(fieldEdgeSize, pair_size, difficulty, hidden_char_secret);
        std::vector<Tile> tiles = playfield.get_playfield();
 
        bool correct_guess = true;
        bool stay_in_game = true;
        std::vector<int> guess_possitions;
        unsigned int guessId = 0;
        int number_of_pairs = tiles.size() / pair_size;
        int correct_guesses = 0;
        unsigned int selectedTileX = 0;
        unsigned int selectedTileY = 0;
        unsigned int cards_turned = 0;

        console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
        console->showScoreCard(number_of_pairs, correct_guesses, stay_in_game);

        double end_time;
        double start_time = GetTickCount();

        do {
            // Flipping back wrong guess and resetting
            if (!correct_guess) {
                Sleep(600); // system("pause>nul"); I think sleep feels better
                for (unsigned int i = 0; i < guess_possitions.size(); i++) {
                    tiles[guess_possitions[i]].flip_tile();
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
                if (GetAsyncKeyState(VK_UP)) {
                    // Check if going up doesn't exit playing field 
                    if (selectedTileY != 0) {
                        selectedTileY--;
                    }
                    // Add keystroke to cheat sequence
                    cheat_sequence.push_back("up");
                } else if (GetAsyncKeyState(VK_DOWN)) {
                    if (selectedTileY < (fieldEdgeSize - 1)) {
                        selectedTileY++;
                    }
                    cheat_sequence.push_back("down");
                } else if (GetAsyncKeyState(VK_LEFT)) {
                    if (selectedTileX != 0) {
                        selectedTileX--;
                    }
                    cheat_sequence.push_back("left");
                } else if (GetAsyncKeyState(VK_RIGHT)) {
                    if (selectedTileX < (fieldEdgeSize - 1)) {
                        selectedTileX++;
                    }
                    cheat_sequence.push_back("right");
                } else if (GetKeyState(66) & 8000) {
                    cheat_sequence.push_back("B");
                } else if (GetKeyState(65) & 8000) {
                    cheat_sequence.push_back("A");
                } else if (GetAsyncKeyState(VK_ESCAPE)) {
                    stay_in_game = false; // Escape to main menu with ESC key
                    break;
                }
                console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);

                // Check for cheat sequences
                if (cheat_sequence.size() > cheat.size()) {
                    cheat_sequence.erase(cheat_sequence.begin());
                }
                if (cheat.size() == cheat_sequence.size()) {
                    for (int i = 0; i < cheat_sequence.size(); i++) {
                        if (cheat[i] == cheat_sequence[i]) {
                            cheat_sequence_correct = true;
                        } else {
                            cheat_sequence_correct = false;
                            break;
                        }
                    }
                    if (cheat_sequence_correct) {
                        show_card_cheat(correct_guesses, cards_turned, &tiles, selectedTileX, selectedTileY);
                    }
                }
                
            } while (!GetAsyncKeyState(VK_RETURN) && !GetAsyncKeyState(VK_SPACE));

            int current_selected_tile = (selectedTileX) + (selectedTileY * playfield.get_playfield_edgesize());

            // Game mechanics
            if (tiles[current_selected_tile].is_flipped()) {
                // Do nothing
            } else {
                tiles[current_selected_tile].flip_tile();
                cards_turned++;
                if (cards_turned == 1) {
                    guessId = tiles[current_selected_tile].get_id();
                    guess_possitions.push_back(current_selected_tile);
                } else if (cards_turned < pair_size && guessId == tiles[current_selected_tile].get_id()) {
                    guess_possitions.push_back(current_selected_tile);
                } else if (cards_turned == pair_size && guessId == tiles[current_selected_tile].get_id()) {
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

        // Save the end-game score and player info
        end_time = GetTickCount();
        double elapsed_time = (end_time - start_time) / 1000;
        Score score((*player).get_name(), difficulty, correct_guesses, number_of_pairs, elapsed_time);
        ScoreCard scorecard;
        scorecard.save_score(&score);

        console->print_endgame_screen(number_of_pairs, correct_guesses);
        return score;
    }

    void Game::setDifficulty(unsigned int difficulty) {
        this->difficulty = difficulty;
        fieldEdgeSize = gameParameters[difficulty][0];
        pair_size = gameParameters[difficulty][1];
    }
}