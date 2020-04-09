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

        Console console;
        console.showPlayField(tiles);

        do {

            for (unsigned int c = 0; c < tiles.size(); c++) {
                char tileChar = ( tiles[c].isTurned() ? tiles[c].getAsciiChar() : tiles[c].getHiddenChar());
                std::cout << tileChar << " ";
                if ((c + 1) % playfield.getFieldEdgeSize() == 0) {
                    std::cout << std::endl;
                }
            }

            if (!correctGuess) {
                Sleep(2000);
                tiles[firstGuessPosition].turnCard();
                tiles[secondGuessPosition].turnCard();

                system("CLS");

                for (unsigned int c = 0; c < tiles.size(); c++) {
                    char tileChar = ( tiles[c].isTurned() ? tiles[c].getAsciiChar() : tiles[c].getHiddenChar());
                    std::cout << tileChar << " ";
                    if ((c + 1) % playfield.getFieldEdgeSize() == 0) {
                        std::cout << std::endl;
                    }
                }
                correctGuess = true;
            }

            std::cout << std::endl;

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
            system("CLS");
        } while (true);

        
        return score;
    }

}