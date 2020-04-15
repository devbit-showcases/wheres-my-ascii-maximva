# Compiler to use
CC=g++

# Compiler flags (iets aan/uit uiteztten met een optie van een command)
CFLAGS=-c -Wall
LDFLAGS=-lgdi32 -lwinmm
	# -c: Compile do not link
	# -Wall: All warnings

# Output binary
EXECUTABLE=main

# target: dependencies
# 	commands

all: $(EXECUTABLE)

$(EXECUTABLE): main.o game.o player.o score.o scorecard.o console.o menu.o playfield.o tile.o
	$(CC) main.o game.o player.o score.o scorecard.o console.o menu.o playfield.o tile.o -o $(EXECUTABLE) $(LDFLAGS)

main.o: ./src/main.cpp
	$(CC) $(CFLAGS) ./src/main.cpp

game.o: ./lib/game.cpp
	$(CC) $(CFLAGS) ./lib/game.cpp

player.o: ./lib/player.cpp
	$(CC) $(CFLAGS) ./lib/player.cpp

score.o: ./lib/score.cpp
	$(CC) $(CFLAGS) ./lib/score.cpp

scorecard.o: ./lib/scorecard.cpp
	$(CC) $(CFLAGS) ./lib/scorecard.cpp

console.o: ./lib/console.cpp
	$(CC) $(CFLAGS) ./lib/console.cpp

menu.o: ./lib/menu.cpp
	$(CC) $(CFLAGS) ./lib/menu.cpp

playfield.o: ./lib/playfield.cpp
	$(CC) $(CFLAGS) ./lib/playfield.cpp

tile.o: ./lib/tile.cpp
	$(CC) $(CFLAGS) ./lib/tile.cpp

clean:
	rm -f *.o $(EXECUTABLE)
