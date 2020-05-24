#pragma once

#include "score.h"
#include "player.h"
#include "playfield.h"
#include <iostream>
#include <windows.h>
#include "console.h"
#include <stdio.h>
#include <algorithm>
#include "scorecard.h"
#include <time.h>
#include "userinput.h"
#include "timer.h"

namespace MyAscii {

    class Game {
        public:
            Game(Player * player, Console * console, unsigned int difficulty);
        
        public:
            void start();

        private:
            void set_game_parameters(unsigned int difficulty);
            void show_5tileflip_cheat(int cards_turned, int selectedTileX, int selectedTileY);
            void show_alltileflip_cheat(void);
            void save_gamescore(double elapsedTime);
            void handle_current_tile(int currentSelectedTile);
            void reset_guess_parameters(void);
            void shortly_turn_tile(int index, int time);
            void check_for_cheats(void);
            void flip_back_wrong_guess(void);
            void clear_returns_and_spaces(void);
            void allow_player_input(void);

        private:
            Player * player;
            Console * console;
            PlayField playfield;
            bool gameFinished = true;
            bool correctGuess = true;
            bool noEscape = true;
            bool konamiCheat = false;
            bool doomCheat = false;
            unsigned int difficulty = 0;
            unsigned int fieldEdgeSize = 0;
            unsigned int setSize = 0;
            unsigned int guessId = 0;
            unsigned int selectedTileX = 0;
            unsigned int selectedTileY = 0;
            unsigned int cardsTurned = 0;
            int correctGuesses = 0;
            int numberOfSets = 0;
            std::vector<int> correctAnswers;
            std::vector<Tile> tiles;
            std::vector<int> guessPossitions;
            std::vector<std::string> konamiCheatSequence = {"up", "up", "down", "down", "left", "right", "left", "right", "B", "A"};
            std::vector<std::string> doomCheatSequence = {"I", "D", "Q", "D"};
            std::vector<std::string> cheatSequence = {};
            // Order: {fieldEdgeSize, pairSize}
            unsigned int gameParameters[5][2] = {
                {4, 2},
                {6, 2},
                {6, 3},
                {9, 3}
            };
    };
};