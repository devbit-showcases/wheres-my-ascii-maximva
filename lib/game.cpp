#include "game.h"

namespace MyAscii {

    /**
     * Creates a new instance of Game
     */
    Game::Game(Player * player, Console * console, unsigned int difficulty) {
        this->player = player;
        this->console = console;
        this->difficulty = difficulty;
        set_game_parameters(difficulty);
        bool dollarHiddenChar = console->dollar_hidden_char_state();
        this->playfield = PlayField(fieldEdgeSize, setSize, difficulty, dollarHiddenChar);
        this->tiles = playfield.get_playfield();
    }


    /**
     * Starts the game
     */
    void Game::start() {
        console->print_playfield(&tiles, fieldEdgeSize, selectedTileX, selectedTileY, difficulty);
        console->show_scorecard(numberOfSets, setSize, correctGuesses, gameFinished, difficulty, (*player).get_name());
        
        Timer clock;
        clock.start();

        do {
            clear_returns_and_spaces();
            if (!correctGuess) flip_back_wrong_guess();
            allow_player_input();
            int currentSelectedTile = (selectedTileX) + (selectedTileY * playfield.get_playfield_edgesize());
            handle_current_tile(currentSelectedTile);
            console->print_playfield(&tiles, fieldEdgeSize, selectedTileX, selectedTileY, difficulty);
            gameFinished = (correctGuesses == numberOfSets ? true : false);
        } while (!gameFinished && noEscape);

        clock.stop();
        double elapsedTime = clock.get_elapsed_seconds();
        save_gamescore(elapsedTime);
        console->print_endgame_screen(numberOfSets, correctGuesses, elapsedTime, player);
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
            } else if (GetKeyState(65) & 8000) {
                cheatSequence.push_back("A");
            } else if (GetKeyState(66) & 8000) {
                cheatSequence.push_back("B");
            } else if (GetKeyState(68) & 8000) {
                cheatSequence.push_back("D");
            } else if (GetKeyState(73) & 8000) {
                cheatSequence.push_back("I");
            } else if (GetKeyState(81) & 8000) {
                cheatSequence.push_back("Q");
            } else if (GetAsyncKeyState(VK_ESCAPE)) {
                noEscape = false; // Escape to main menu with ESC key
                break;
            }

            console->print_playfield(&tiles, fieldEdgeSize, selectedTileX, selectedTileY, difficulty);
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
        console->print_playfield(&tiles, fieldEdgeSize, selectedTileX, selectedTileY, difficulty);
        reset_guess_parameters();
        correctGuess = true;
    }


    /**
     * Check if a cheat sequence has been entered
     */
    void Game::check_for_cheats(void) {
        const unsigned int MAX_CHEAT_LENGTH = 10;
        if (cheatSequence.size() > MAX_CHEAT_LENGTH) {
            cheatSequence.erase(cheatSequence.begin());
        }

        if (cheatSequence.size() >= konamiCheatSequence.size()) {
            for (unsigned int i = 0; i < konamiCheatSequence.size(); i++) {
                if (konamiCheatSequence[i] == cheatSequence[i]) {
                    konamiCheat = true;
                } else {
                    konamiCheat = false;
                    break;
                }
            }

            if (konamiCheat) {
                cheatSequence.clear();
                show_5tileflip_cheat(cardsTurned, selectedTileX, selectedTileY);
            }
        }

        if (cheatSequence.size() >= doomCheatSequence.size()) {
            for (unsigned int j = 0; j < cheatSequence.size() - doomCheatSequence.size() + 2; j++) {
                for (unsigned int i = j; i < j + doomCheatSequence.size(); i++) {

                    if (doomCheatSequence[i - j] == cheatSequence[i]) {
                        doomCheat = true;
                    } else {
                        doomCheat = false;
                        break;
                    }
                }
                if (doomCheat) {
                    cheatSequence.clear();
                    show_alltileflip_cheat();
                    break;
                }
            }

        }

    }


    /**
     * Turn a tile for a short period of time
     */
    void Game::shortly_turn_tile(int index, int time) {
        tiles[index].flip_tile();
        console->print_playfield(&tiles, fieldEdgeSize, selectedTileX, selectedTileY, difficulty);
        Sleep(time);
        tiles[index].flip_tile();
        console->print_playfield(&tiles, fieldEdgeSize, selectedTileX, selectedTileY, difficulty);
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
                console->show_scorecard(numberOfSets, setSize, correctGuesses, gameFinished, difficulty, (*player).get_name());
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


    /**
     * Flips 5 random tiles for a short period of time
     */
    void Game::show_5tileflip_cheat(int cardsTurned, int selectedTileX, int selectedTileY) {
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
     * Flips all tiles for a brief period of time
     */
    void Game::show_alltileflip_cheat(void) {
        unsigned int cardsToTurn = (fieldEdgeSize * fieldEdgeSize);
        for (unsigned int i = 0; i < cardsToTurn; i++) {
            if (!tiles[i].is_flipped()) {
                shortly_turn_tile(i, 300);
            }
        }
    }
}