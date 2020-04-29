#include "game.h"

namespace MyAscii {

    Game::Game(Player * player, Console * console, unsigned int difficulty) {
        this->player = player;
        this->console = console;
        this->difficulty = difficulty;
        set_game_parameters(difficulty);
        bool hidden_char_secret = console->hiddenCharState();
        this->playfield = PlayField(fieldEdgeSize, pair_size, difficulty, hidden_char_secret);
        this->tiles = playfield.get_playfield();
    }

    void Game::show_card_cheat(int cards_turned, int selectedTileX, int selectedTileY) {
        unsigned int cards_to_turn = (fieldEdgeSize * fieldEdgeSize) - (correct_guesses * pair_size) - cards_turned;
        std::vector<int> used_indexes = {};

        for (unsigned int i = 0; (i < cards_to_turn) && (i < 5); i++) {
            int random_index = rand() % (fieldEdgeSize * fieldEdgeSize);

            // Check if tile is turned or has been used in this show of cards allready
            while (tiles[random_index].is_flipped() || std::count(used_indexes.begin(), used_indexes.end(), random_index)) {
                random_index = rand() % (fieldEdgeSize * fieldEdgeSize);
            }
            used_indexes.push_back(random_index);
            shortly_turn_tile(random_index, 500);
        }
    }


    /**
     * Starts the game
     */
    void Game::start() {
        console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
        console->showScoreCard(number_of_pairs, correct_guesses, stay_in_game);
        double end_time;
        double start_time = GetTickCount();

        do {
            clear_returns_spaces();
            if (!correct_guess) flip_back_wrong_guess();
            select_tile();
            int currentSelectedTile = (selectedTileX) + (selectedTileY * playfield.get_playfield_edgesize());
            handle_current_tile(currentSelectedTile);
            console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
            stay_in_game = (correct_guesses == number_of_pairs ? false : true);
        } while (stay_in_game);

        end_time = GetTickCount();
        save_gamescore(end_time, start_time);
        console->print_endgame_screen(number_of_pairs, correct_guesses);
    }


    /**
     * Allow player to select a tile
     */
    void Game::select_tile(void) {
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
            check_for_cheats();
            
        } while (!GetAsyncKeyState(VK_RETURN) && !GetAsyncKeyState(VK_SPACE));
    }


    /**
     * Clears any remaning returns or spaces
     */
    void Game::clear_returns_spaces(void) {
        GetAsyncKeyState(VK_RETURN);
        GetAsyncKeyState(VK_SPACE);
    }


    /**
     * Flips back all tiles if guess was wrong
     */
    void Game::flip_back_wrong_guess(void) {
        Sleep(600);
        for (unsigned int i = 0; i < guess_possitions.size(); i++) {
            tiles[guess_possitions[i]].flip_tile();
        }
        console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
        reset_guess_parameters();
        correct_guess = true;
    }


    /**
     * Check if a cheat sequence has been entered
     */
    void Game::check_for_cheats(void) {
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
                show_card_cheat(cardsTurned, selectedTileX, selectedTileY);
            }
        }
    }


    /**
     * Turn a tile for a short period of time
     */
    void Game::shortly_turn_tile(int index, int time) {
        tiles[index].flip_tile();
        console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
        Sleep(time);
        tiles[index].flip_tile();
        console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
    }


    /**
     * Deside what to do with a tile thats selected
     */
    void Game::handle_current_tile(int currentSelectedTile) {
        if (tiles[currentSelectedTile].is_flipped()) {

        } else {
            tiles[currentSelectedTile].flip_tile();
            cardsTurned++;
            if (cardsTurned == 1) {
                guessId = tiles[currentSelectedTile].get_id();
                guess_possitions.push_back(currentSelectedTile);
            } else if (cardsTurned < pair_size && guessId == tiles[currentSelectedTile].get_id()) {
                guess_possitions.push_back(currentSelectedTile);
            } else if (cardsTurned == pair_size && guessId == tiles[currentSelectedTile].get_id()) {
                correct_guesses++;
                console->showScoreCard(number_of_pairs, correct_guesses, stay_in_game);
                reset_guess_parameters();
            } else {
                guess_possitions.push_back(currentSelectedTile);
                correct_guess = false;
            }
        }
    }


    /**
     * Reset parameters handling guesses for a set of tiles
     */
    void Game::reset_guess_parameters(void) {
        cardsTurned = 0;
        guessId = 0;
        guess_possitions.clear();
    }


    /**
     * Save the games score
     */
    void Game::save_gamescore(double endTime, double startTime) {
        double elapsed_time = (endTime - startTime) / 1000;
        Score score((*player).get_name(), difficulty, correct_guesses, number_of_pairs, elapsed_time);
        ScoreCard scorecard;
        scorecard.save_score(&score);
    }


    /**
     * Set the game parameters
     */
    void Game::set_game_parameters(unsigned int difficulty) {
        fieldEdgeSize = gameParameters[difficulty][0];
        pair_size = gameParameters[difficulty][1];
        number_of_pairs = (fieldEdgeSize * fieldEdgeSize) / pair_size;
    }
}