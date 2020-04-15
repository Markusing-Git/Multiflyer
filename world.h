#ifndef world_h
#define world_h

#include <stdio.h>
#include <SDL2/SDL.h>
#include "player.h"

void worldCollision(SDL_Rect* aPlayerPos, Player aPLayer, int screenWidth, int  screenHeight);

#endif /* world_h */
