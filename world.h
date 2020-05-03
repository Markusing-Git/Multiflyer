#ifndef world_h
#define world_h

#include <stdio.h>
#include <SDL2/SDL.h>
#include "player.h"
#include "loadMedia.h"

//Scans if player has collided with world limitations
void worldCollision(SDL_Rect* aPlayerPos, Player aPLayer, int screenWidth, int  screenHeight);

//Makes the background scroll to the left, takes a media for backround image, and the address of an offset variable for movement
void scrollBackground(LoadMedia aMedia, int* aOffset, int w, int h);

#endif /* world_h */
