#ifndef player_h
#define player_h

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include "constants.h"

//en ADT för att skapa en spelare

typedef struct playerType* Player;

//creates a player struct
Player createPlayer(int x, int y);

//renders player dead or alive
void renderPlayer(SDL_Renderer* renderer, SDL_Texture* playerTex, SDL_Texture* splashTex, SDL_Rect* playerPos, Player aPLayer,
	SDL_Rect* playerSprites, SDL_Rect* splashSprites, int playerFrame, int splashFrame, Mix_Chunk* electricShock, int* nrOfSoundEffects);

//creates a new player and adds to the list of players
void newPlayer(Player playerList[], Player aPlayer, int* playerCount);

//returns adress of player position 
SDL_Rect* getPlayerPosAdr(Player aPlayer);

//returns player players status false: dead true: alive
bool getPlayerStatus(Player aPLayer);

//sets the status of player false: dead true: alive
void setPlayerStatus(Player aPlayer, bool deadOrAlive);

//sets cordinates of a players x:y:w:h, value represents the value to be set
void setPlayerPoint(Player aPlayer, char cord, int value);

//returns the value of player cordinate. cord: x:y:w:h
int getPlayerPoint(Player aPlayer, char cord);

//moves a player
void movePlayerUp(Player aPlayer, int speed);
void movePlayerDown(Player aPlayer, int speed);
void movePlayerLeft(Player aPlayer, int speed);
void movePlayerRight(Player aPlayer, int speed);

//frees player structs from heap, params: list of players and amount
void freePlayers(Player playerList[], int playerCount);

#endif /* player_h */