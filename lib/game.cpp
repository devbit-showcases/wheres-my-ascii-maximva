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

        console->showPlayField(&tiles, fieldEdgeSize);

        do {

            if (!correctGuess) {
                Sleep(1500);
                tiles[firstGuessPosition].turnCard();
                tiles[secondGuessPosition].turnCard();
                console->showPlayField(&tiles, fieldEdgeSize);
                correctGuess = true;
            }

            COORD cursorPosition;
            cursorPosition.X = 100;
            cursorPosition.Y = 5;

            SetConsoleCursorPosition(GetStdHandle(STD_INPUT_HANDLE), cursorPosition);

            std::cout << "Please enter x coordinate: ";
            int x;
            std::cin >> x;

            std::cout << "Please enter y coordinate: ";
            int y;
            std::cin >> y;

            int position = (x-1) + ((y-1) * playfield.getFieldEdgeSize());

            bool allreadyCorrect = std::find(correctAnswers.begin(), correctAnswers.end(), tiles[position].getId()) != correctAnswers.end();

            if (allreadyCorrect) {
                // maybe do something if player clicks on allready guesse tile
            } else {
                if (firstGuess) {
                    guessId = tiles[position].getId();
                    firstGuessPosition = position;
                    firstGuess = !firstGuess;
                    tiles[position].turnCard();
                } else {
                    if (tiles[position].getId() == guessId) {
                        tiles[position].turnCard();
                        firstGuess = !firstGuess;
                        correctAnswers.push_back(guessId);
                    } else {
                        tiles[position].turnCard();
                        secondGuessPosition = position;
                        firstGuess = !firstGuess;
                        correctGuess = false;
                    }
                }
            }
            console->showPlayField(&tiles, fieldEdgeSize);
        } while (true);

        
        return score;
    }

    void Game::setDifficulty(unsigned int difficulty) {
        this->difficulty = difficulty;
        fieldEdgeSize = gameParameters[difficulty][0];
        pairSize = gameParameters[difficulty][1];
    }
}