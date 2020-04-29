#ifndef LOADMEDIA_H
#define LOADMEDIA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "constants.h"

struct loadMedia {
	SDL_Texture* flyTrapTex;
	SDL_Texture* flyTex;
	SDL_Texture* flySplashTex;
	SDL_Texture* backgroundTex;

	SDL_Surface* flyTrapSurface;
	SDL_Surface* flySurface;
	SDL_Surface* flySplashSurface;
	SDL_Surface* backgroundSurface;

	SDL_Rect startFlyBlue[PLAYER_FRAMES];
	SDL_Rect startFlyGreen[PLAYER_FRAMES];
	SDL_Rect startFlyRed[PLAYER_FRAMES];
	SDL_Rect startFlyYellow[PLAYER_FRAMES];
	SDL_Rect splashSprites[SPLASH_FRAMES];
	SDL_Rect scrollingBackground[2];
	
	Mix_Chunk* electricShock;
	Mix_Music* backgroundMusic;
};
typedef struct loadMedia* LoadMedia;

LoadMedia loadMedia(SDL_Renderer* renderer, bool* running);

#endif // !LOADMEDIA_H