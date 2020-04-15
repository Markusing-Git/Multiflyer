# A simple Makefile for compiling small SDL projects

theGame: main.o game_engine.o LoadMenu.o Network.o obstacle.o player.o world.o client_game_engine.o 
	gcc -o theGame main.o game_engine.o LoadMenu.o Network.o obstacle.o player.o world.o client_game_engine.o -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf
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
world.o: world.c
	gcc -c world.c
client_game_engine.o: client_game_engine.c
	gcc -c client_game_engine.c

