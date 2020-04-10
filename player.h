#ifndef player_h
#define player_h

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#define PLAYER_FRAMES 3 //Antal frames i spritesheet
#define SPLASH_FRAMES 14

//en ADT för att skapa en spelare
//getPlayerPositionX returnerar spelarens x cordinat
//getPlayerPositionY returnerar spelarens y cordinat
//drawPlayer ritar ut spelaren på skärmen (Kanske borde tillhöra annan source fil t.ex en som heter game??)

struct playerType {
	SDL_Rect playerPos;
	bool alive;
};

typedef struct playerType* Player;

//creates a player struct
Player createPlayer(int x, int y);

//renders player dead or alive
void renderPlayer(SDL_Renderer* renderer, SDL_Texture* playerTex, SDL_Texture* splashTex, SDL_Rect* playerPos, Player aPLayer,
	SDL_Rect* playerSprites, SDL_Rect* splashSprites, int playerFrame, int splashFrame);


#endif /* player_h */