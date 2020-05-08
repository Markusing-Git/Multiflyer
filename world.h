#ifndef world_h
#define world_h

#include <stdio.h>
#include <SDL2/SDL.h>
#include "player.h"
#include "loadMedia.h"

#define POWERUP_SPEED 2

typedef struct powerUp_type* PowerUp;

//Scans if player has collided with world limitations
void worldCollision(SDL_Rect* aPlayerPos, Player aPLayer, int screenWidth, int  screenHeight);

//Makes the background scroll to the left, takes a media for backround image, and the address of an offset variable for movement
void scrollBackground(LoadMedia aMedia, int* aOffset, int w, int h);

PUBLIC PowerUp serverSpawnPowerUp(int screenWidth, int screenHeight);

PUBLIC void powerUpTick(PowerUp aPowerUp, int screenWidth, int screenHeight);

PUBLIC void renderPowerUp(SDL_Renderer* renderer, PowerUp aPowerUp, LoadMedia media);

PUBLIC int powerUpConsumed(Player playerList[], PowerUp aPowerUp, int playerCount);

#endif /* world_h */
