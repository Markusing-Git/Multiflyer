# A simple Makefile for compiling small SDL projects

theGame: main.o
	gcc -o theGame main.o LoadMenu.o game_engine.o -player.o -lSDL2 -lSDL2_image
main.o: main.c
	gcc -c main.c
LoadMenu.o: LoadMenu.c
	gcc -c LoadMenu.c
player.o: player.c
	gcc -c player.c
game_engine.o: game_engine.c
	gcc -c game_engine.c

all: $(EXEC)
