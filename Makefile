# A simple Makefile for compiling small SDL projects

<<<<<<< HEAD
theGame: main.o game_engine.o LoadMenu.o Network.o obstacle.o player.o
	gcc -o theGame main.o game_engine.o LoadMenu.o Network.o obstacle.o player.o -lSDL2 -lSDL2_image -lSDL2_net
main.o: main.c
	gcc -c main.c
game_engine.o: game_engine.c
	gcc -c game_engine.c
LoadMenu.o: LoadMenu.c
	gcc -c LoadMenu.c
Network.o: Network.c
	gcc -c Network.c
obstacle.o: obstacle.c
	gcc -c obstacle.c
player.o: player.c
	gcc -c player.c
=======
theGame: main.o
	gcc -o theGame main.o LoadMenu.o game_engine.o player.o -lSDL2 -lSDL2_image
main.o: main.c
	gcc -c main.c
LoadMenu.o: LoadMenu.c
	gcc -c LoadMenu.c
player.o: player.c
	gcc -c player.c
game_engine.o: game_engine.c
	gcc -c game_engine.c

>>>>>>> 1dd08823a12b8cf810f4778c3a48811eabb2415b
all: $(EXEC)
