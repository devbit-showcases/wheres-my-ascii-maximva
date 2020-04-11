#include "game.h"

namespace MyAscii {

    Game::Game(Player * player, Console * console) {
        this->player = player;
        this->console = console;
    }

    Score Game::start(unsigned int difficulty) {
        Score score; // Doesn't do anything right now
        setDifficulty(difficulty);
        PlayField playfield(fieldEdgeSize, pairSize, difficulty);
        std::vector<Tile> tiles = playfield.getPlayField();

        bool firstGuess = true;
        bool correctGuess = true;
        int firstGuessPosition = 0;
        int secondGuessPosition = 0;
        int guessId = 0;
        int number_of_pairs = tiles.size() / pairSize;
        int correct_guesses = 0;
        int selectedTileX = 0;
        int selectedTileY = 0;

        console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
        console->showScoreCard(number_of_pairs, correct_guesses);

        do {
            if (!correctGuess) {
                Sleep(700);     // Or system("pause>nul");  ... don't now what feels better...
                tiles[firstGuessPosition].turnCard();
                tiles[secondGuessPosition].turnCard();
                console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
                correctGuess = true;
            }

            COORD cursorPosition;
            cursorPosition.X = 100;
            cursorPosition.Y = 5;

            GetAsyncKeyState(VK_RETURN); // Clear any remaining RETURNs
            
            // Allow arrow/return key selection of tiles
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
                }
            } while (!GetAsyncKeyState(VK_RETURN) && !GetAsyncKeyState(VK_SPACE));

            int position = (selectedTileX) + (selectedTileY * playfield.getFieldEdgeSize());
            bool allreadyCorrect = std::find(correctAnswers.begin(), correctAnswers.end(), tiles[position].getId()) != correctAnswers.end();

            if (allreadyCorrect) {
                // maybe do something if player clicks on allready guessed tile
            } else {
                if (firstGuess) {
                    guessId = tiles[position].getId();
                    firstGuessPosition = position;
                    firstGuess = !firstGuess;
                    tiles[position].turnCard();
                } else {
                    if (position == firstGuessPosition) {
                        // maybe do something if tile is played twice
                    } else if (tiles[position].getId() == guessId) {
                        tiles[position].turnCard();
                        firstGuess = !firstGuess;
                        correctAnswers.push_back(guessId);
                        correct_guesses++;
                        console->showScoreCard(number_of_pairs, correct_guesses);
                    } else {
                        tiles[position].turnCard();
                        secondGuessPosition = position;
                        firstGuess = !firstGuess;
                        correctGuess = false;
                    }
                }
            }
            console->showPlayField(&tiles, fieldEdgeSize, selectedTileX, selectedTileY);
        } while (true);

        return score;
    }

    void Game::setDifficulty(unsigned int difficulty) {
        this->difficulty = difficulty;
        fieldEdgeSize = gameParameters[difficulty][0];
        pairSize = gameParameters[difficulty][1];
    }
}