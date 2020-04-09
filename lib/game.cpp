#include "game.h"

namespace MyAscii {

    Game::Game(Player * player, unsigned int difficulty) {
        this->player = player;
        this->difficulty = difficulty;
    }

    Score Game::start(void) {
        Score score;
        unsigned int fieldEdgeSize = gameParameters[difficulty][0];
        unsigned int pairSize = gameParameters[difficulty][1];

        PlayField playfield(fieldEdgeSize, pairSize);
        std::vector<Tile> tiles = playfield.getPlayField();

        bool firstGuess = true;
        bool correctGuess = true;
        int firstGuessPosition = 0;
        int secondGuessPosition = 0;
        int guessId = 0;

        Console console("Where's my ASCII");
        console.showPlayField(&tiles, fieldEdgeSize);

        do {

            if (!correctGuess) {
                Sleep(1500);
                tiles[firstGuessPosition].turnCard();
                tiles[secondGuessPosition].turnCard();

                console.showPlayField(&tiles, fieldEdgeSize);

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

            if (firstGuess) {
                guessId = tiles[position].getId();
                firstGuessPosition = position;
                firstGuess = !firstGuess;
                tiles[position].turnCard();
            } else {
                if (tiles[position].getId() == guessId) {
                    tiles[position].turnCard();
                    firstGuess = !firstGuess;
                } else {
                    tiles[position].turnCard();
                    secondGuessPosition = position;
                    firstGuess = !firstGuess;
                    correctGuess = false;
                }
            }
            console.showPlayField(&tiles, fieldEdgeSize);
        } while (true);

        
        return score;
    }

}