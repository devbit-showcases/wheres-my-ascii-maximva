#include "game.h"

namespace MyAscii {

    Game::Game(Player * player, Console * console, unsigned int difficulty) {
        this->player = player;
        this->console = console;
        this->difficulty = difficulty;
        set_game_parameters(difficulty);
        bool hidden_char_secret = console->hidden_char_state();
        this->playfield = PlayField(fieldEdgeSize, setSize, difficulty, hidden_char_secret);
        this->tiles = playfield.get_playfield();
    }

    void Game::show_card_cheat(int cardsTurned, int selectedTileX, int selectedTileY) {
        unsigned int cards_to_turn = (fieldEdgeSize * fieldEdgeSize) - (correctGuesses * setSize) - cardsTurned;
        std::vector<int> usedIndexes = {};

        for (unsigned int i = 0; (i < cards_to_turn) && (i < 5); i++) {
            int randomIndex = rand() % (fieldEdgeSize * fieldEdgeSize);

            // Check if tile is turned or has been used in this show of cards allready
            while (tiles[randomIndex].is_flipped() || std::count(usedIndexes.begin(), usedIndexes.end(), randomIndex)) {
                randomIndex = rand() % (fieldEdgeSize * fieldEdgeSize);
            }
            usedIndexes.push_back(randomIndex);
            shortly_turn_tile(randomIndex, 500);
        }
    }


    /**
     * Starts the game
     */
    void Game::start() {
        console->print_playfield(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
        console->show_scorecard(numberOfSets, setSize, correctGuesses, unCompleteGame);
        
        double endTime;
        double startTime = GetTickCount();

        do {
            clear_returns_and_spaces();
            if (!correctGuess) flip_back_wrong_guess();
            allow_player_input();
            int currentSelectedTile = (selectedTileX) + (selectedTileY * playfield.get_playfield_edgesize());
            handle_current_tile(currentSelectedTile);
            console->print_playfield(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
            unCompleteGame = (correctGuesses == numberOfSets ? false : true);
        } while (unCompleteGame && noEscape);

        endTime = GetTickCount();
        double elapsedTime = (endTime - startTime) / 1000;
        save_gamescore(elapsedTime);
        console->print_endgame_screen(numberOfSets, correctGuesses, elapsedTime);
    }


    /**
     * Allow player to select a tile
     */
    void Game::allow_player_input(void) {
        do {
            system("pause>nul");    // pause after keystroke
            if (GetAsyncKeyState(VK_UP)) {
                // Check if going up doesn't exit playing field 
                if (selectedTileY != 0) {
                    selectedTileY--;
                }
                // Add keystroke to cheat sequence
                cheatSequence.push_back("up");
            } else if (GetAsyncKeyState(VK_DOWN)) {
                if (selectedTileY < (fieldEdgeSize - 1)) {
                    selectedTileY++;
                }
                cheatSequence.push_back("down");
            } else if (GetAsyncKeyState(VK_LEFT)) {
                if (selectedTileX != 0) {
                    selectedTileX--;
                }
                cheatSequence.push_back("left");
            } else if (GetAsyncKeyState(VK_RIGHT)) {
                if (selectedTileX < (fieldEdgeSize - 1)) {
                    selectedTileX++;
                }
                cheatSequence.push_back("right");
            } else if (GetKeyState(66) & 8000) {
                cheatSequence.push_back("B");
            } else if (GetKeyState(65) & 8000) {
                cheatSequence.push_back("A");
            } else if (GetAsyncKeyState(VK_ESCAPE)) {
                noEscape = false; // Escape to main menu with ESC key
                break;
            }

            console->print_playfield(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
            check_for_cheats();
            
        } while (!GetAsyncKeyState(VK_RETURN) && !GetAsyncKeyState(VK_SPACE));
    }


    /**
     * Clears any remaning returns or spaces
     */
    void Game::clear_returns_and_spaces(void) {
        GetAsyncKeyState(VK_RETURN);
        GetAsyncKeyState(VK_SPACE);
    }


    /**
     * Flips back all tiles if guess was wrong
     */
    void Game::flip_back_wrong_guess(void) {
        Sleep(600);
        for (unsigned int i = 0; i < guessPossitions.size(); i++) {
            tiles[guessPossitions[i]].flip_tile();
        }
        console->print_playfield(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
        reset_guess_parameters();
        correctGuess = true;
    }


    /**
     * Check if a cheat sequence has been entered
     */
    void Game::check_for_cheats(void) {
        if (cheatSequence.size() > cheat.size()) {
            cheatSequence.erase(cheatSequence.begin());
        }
        if (cheat.size() == cheatSequence.size()) {
            for (unsigned int i = 0; i < cheatSequence.size(); i++) {
                if (cheat[i] == cheatSequence[i]) {
                    correctCheatSequence = true;
                } else {
                    correctCheatSequence = false;
                    break;
                }
            }
            if (correctCheatSequence) {
                show_card_cheat(cardsTurned, selectedTileX, selectedTileY);
            }
        }
    }


    /**
     * Turn a tile for a short period of time
     */
    void Game::shortly_turn_tile(int index, int time) {
        tiles[index].flip_tile();
        console->print_playfield(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
        Sleep(time);
        tiles[index].flip_tile();
        console->print_playfield(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
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
                guessPossitions.push_back(currentSelectedTile);
            } else if (cardsTurned < setSize && guessId == tiles[currentSelectedTile].get_id()) {
                guessPossitions.push_back(currentSelectedTile);
            } else if (cardsTurned == setSize && guessId == tiles[currentSelectedTile].get_id()) {
                correctGuesses++;
                console->show_scorecard(numberOfSets, setSize, correctGuesses, unCompleteGame);
                reset_guess_parameters();
            } else {
                guessPossitions.push_back(currentSelectedTile);
                correctGuess = false;
            }
        }
    }


    /**
     * Reset parameters handling guesses for a set of tiles
     */
    void Game::reset_guess_parameters(void) {
        cardsTurned = 0;
        guessId = 0;
        guessPossitions.clear();
    }


    /**
     * Save the games score
     */
    void Game::save_gamescore(double elapsedTime) {
        Score score((*player).get_name(), difficulty, correctGuesses, numberOfSets, elapsedTime);
        ScoreCard scorecard;
        scorecard.save_score(&score);
    }


    /**
     * Set the game parameters
     */
    void Game::set_game_parameters(unsigned int difficulty) {
        fieldEdgeSize = gameParameters[difficulty][0];
        setSize = gameParameters[difficulty][1];
        numberOfSets = (fieldEdgeSize * fieldEdgeSize) / setSize;
    }
}