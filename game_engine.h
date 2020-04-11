#ifndef game_engine_h
#define game_engine_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "obstacle.h"
#include "Network.h"
#include "world.h"

#define PLAYER_FRAMES 3 //Antal frames i spritesheet
#define SPLASH_FRAMES 14
#define TIME_DELAY 100
#define MAX_PLAYERS 3

//params a renderer, screen width, screen height
bool startGame(SDL_Renderer* renderer, int w, int h);
bool startClientGame(SDL_Renderer* renderer, int w, int h);

#endif /*game_engine_h*/