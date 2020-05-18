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

//initiates a empty powerUp struct
PUBLIC PowerUp initPowerUp(void);

//spawns a serverside powerUp with a random powerType and direction
PUBLIC PowerUp serverSpawnPowerUp(int screenWidth, int screenHeight, PowerUp oldPowerUp);

//spawns a clientside poweUp used for networkPurpuses
PUBLIC PowerUp clientSpawnPowerUp(SDL_Rect powerUpRect, int powerUpDir, int powerUpType);

//returns position and size of powerUp
PUBLIC SDL_Rect getPowerUpRect(PowerUp aPowerUp);

//returns powerUpDirection
PUBLIC int getPowerUpDir(PowerUp aPowerUp);

//returns powerUp type
PUBLIC int getPowerUpType(PowerUp aPowerUp);

//moves powerUp around and bounces of screen.
PUBLIC void powerUpTick(PowerUp aPowerUp, int screenWidth, int screenHeight);

//renders powerUp if there are any active
PUBLIC void renderPowerUp(SDL_Renderer* renderer, PowerUp aPowerUp, LoadMedia media, int *coinFrames);

//checkes if player has collided with a powerUp and consumes it
PUBLIC int powerUpConsumed(Player playerList[], PowerUp aPowerUp, int playerCount, Uint32 *powerDurationTimer);

#endif /* world_h */
