#ifndef player_h
#define player_h
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include "constants.h"
#include "loadMedia.h"

typedef enum powerType {
	life,
	shield,
	attack,
	none
} PowerType;

//en ADT f�r att skapa en spelare

typedef struct playerType* Player;

//creates a player struct
PUBLIC Player createPlayer(int x, int y);

//renders players dead or alive
PUBLIC void renderPlayers(SDL_Renderer* renderer, Player playerList[], int playerFrame, int splashFrame[], int* nrOfSoundEffects, int playerCount, LoadMedia media);

//creates a new player and adds to the list of players
PUBLIC void initPlayers(Player playerList[], int playerCount);

//returns adress of player position 
PUBLIC SDL_Rect* getPlayerPosAdr(Player aPlayer);

//returns player players status false: dead true: alive
PUBLIC bool getPlayerStatus(Player aPLayer);

//sets the consumed power of player
PUBLIC void setPlayerPower(Player aPlayer, PowerType aPowerType);

//sets the status of player false: dead true: alive
PUBLIC void setPlayerStatus(Player aPlayer, bool deadOrAlive);

//sets cordinates of a players x:y:w:h, value represents the value to be set
PUBLIC void setPlayerPoint(Player aPlayer, char cord, int value);

//returns the value of player cordinate. cord: x:y:w:h
PUBLIC int getPlayerPoint(Player aPlayer, char cord);

//Returns the score of a player
PUBLIC int getPlayerScore(Player aPlayer);

//Plays the sound effect
void getSoundEffect(int alive, Mix_Chunk* soundEffect);

//moves a player
PUBLIC void movePlayerUp(Player aPlayer, int speed);
PUBLIC void movePlayerDown(Player aPlayer, int speed);
PUBLIC void movePlayerLeft(Player aPlayer, int speed);
PUBLIC void movePlayerRight(Player aPlayer, int speed);

//frees player structs from heap, params: list of players and amount
PUBLIC void freePlayers(Player playerList[], int playerCount);

//Adds one to a players score
PUBLIC void addScore(Player aPlayer);

//Renders player score
PUBLIC void renderScore(Player aPlayer, LoadMedia media, SDL_Renderer* renderer, Fonts fonts);

/*checks if all players are dead and game is over,
params: list of players and how many
Uint32 delay for lastplayer splash effects
*/
PUBLIC bool gameOver(Player playerList[], int playerCount, Uint32* delay, bool* delayFlag);

//Checks if there is contact between players in case of a push
PUBLIC int playerContact(SDL_Rect* playerPos, SDL_Rect* opponentPos);


#endif /* player_h */