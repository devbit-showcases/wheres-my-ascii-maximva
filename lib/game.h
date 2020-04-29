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

namespace MyAscii {

    class Game {
        public:
            Game(Player * player, Console * console, unsigned int difficulty);
        
        public:
            void start();
            void set_game_parameters(unsigned int difficulty);

        private:
            void show_card_cheat(int cards_turned, int selectedTileX, int selectedTileY);
            void save_gamescore(double endTime, double startTime);
            void handle_current_tile(int currentSelectedTile);
            void reset_guess_parameters(void);
            void shortly_turn_tile(int index, int time);
            void check_for_cheats(void);
            void flip_back_wrong_guess(void);
            void clear_returns_spaces(void);
            void select_tile(void);

        private:
            Player * player;
            Console * console;
            PlayField playfield;
            unsigned int difficulty = 0;
            unsigned int fieldEdgeSize = 0;
            unsigned int pair_size = 0;
            std::vector<int> correctAnswers;
            std::vector<Tile> tiles;
            int correct_guesses = 0;
            int number_of_pairs = 0;
            bool stay_in_game = true;
            bool correct_guess = true;
            std::vector<int> guess_possitions;
            unsigned int guessId = 0;
            unsigned int selectedTileX = 0;
            unsigned int selectedTileY = 0;
            unsigned int cardsTurned = 0;


            std::vector<std::string> cheat = {"up", "up", "down", "down", "left", "right", "left", "right", "B", "A"};
            std::vector<std::string> cheat_sequence = {};
            bool cheat_sequence_correct = false;

            // Order: {fieldEdgeSize, pairSize}
            unsigned int gameParameters[5][2] = {
                {4, 2},
                {6, 3},
                {6, 3}, // Same as before but with possible double characters  AND TIMER
                {9, 3}  // Insane, large, tripple tile and possible double characters  AND TIMER
            };
    };
};