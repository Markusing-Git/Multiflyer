#ifndef STORE_H
#define STORE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <SDL2/SDL_mixer.h>
#include "loadMedia.h"
#include "constants.h"
#include "player.h"

void store(SDL_Renderer* renderer, LoadMedia media, Fonts fonts, int* coins, bool* skinChoice[]);

#endif // !STORE_H
