#ifndef game_engine_h
#define game_engine_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//params a renderer, screen width, screen height
bool startGame(SDL_Renderer* renderer, int w, int h);

#endif /*game_engine_h*/