#include "game.h"

namespace MyAscii {

    Game::Game(Player * player, Console * console) {
        this->player = player;
        this->console = console;
    }

    Score Game::start(unsigned int difficulty) {
        Score score; // Doesn't do anything right now
        setDifficulty(difficulty);
        bool hidden_char_secret = console->hiddenCharState();
        PlayField playfield(fieldEdgeSize, pair_size, difficulty, hidden_char_secret);
        std::vector<Tile> tiles = playfield.getPlayField();
 
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

                // Now come the cheats / powerups
                // number key values top: 48 - 57   Keypad: 96 - 105
                if (GetKeyState(48) & 8000 || GetKeyState(96) & 8000) {
                    system("pause>nul");
                    if (GetKeyState(VK_UP) & 8000) {
                        system("pause>nul");
                        if (GetKeyState(VK_UP) & 8000) {
                            system("pause>nul");
                            if (GetKeyState(VK_DOWN) & 8000) {
                                system("pause>nul");
                                if (GetKeyState(VK_DOWN) & 8000) {
                                    system("pause>nul");
                                    if (GetKeyState(VK_LEFT) & 8000) {
                                        system("pause>nul");
                                        if (GetKeyState(VK_RIGHT) & 8000) {
                                            system("pause>nul");
                                            if (GetKeyState(VK_LEFT) & 8000) {
                                                system("pause>nul");
                                                if (GetKeyState(VK_RIGHT) & 8000) {
                                                    system("pause>nul");
                                                    if (GetKeyState(66) & 8000) {
                                                        system("pause>nul");
                                                        if (GetKeyState(65) & 8000) {
                                                            unsigned int cards_to_turn = (fieldEdgeSize * fieldEdgeSize) - (correct_guesses * pair_size) - cards_turned;
                                                            for (unsigned int i = 0; (i < cards_to_turn) && (i < 5); i++) {
                                                                int random_index = rand() % (fieldEdgeSize * fieldEdgeSize);
                                                                while (tiles[random_index].isTurned()) {
                                                                    random_index = rand() % (fieldEdgeSize * fieldEdgeSize);
                                                                }
                                                                tiles[random_index].turnCard();
                                                                console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
                                                                tiles[random_index].turnCard();
                                                                Sleep(500);
                                                                console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    // Make sure there's no arrow keys presses left in buffer
                    // Otherwise if cheat is not entered fully arrow keys will behave weird afterwards
                    GetAsyncKeyState(VK_UP);
                    GetAsyncKeyState(VK_UP);
                    GetAsyncKeyState(VK_DOWN);
                    GetAsyncKeyState(VK_DOWN);
                    GetAsyncKeyState(VK_LEFT);
                    GetAsyncKeyState(VK_LEFT);
                    GetAsyncKeyState(VK_RIGHT);
                    GetAsyncKeyState(VK_RIGHT);
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

        // Save the end-game score and player info
        end_time = GetTickCount();
        double elapsed_time = (end_time - start_time) / 1000;
        score.set_score((*player).get_name(), difficulty, correct_guesses, number_of_pairs, elapsed_time);
        ScoreCard scorecard;
        scorecard.save_score(&score);

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