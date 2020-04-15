#ifndef LOADMEDIA_H
#define LOADMEDIA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define PLAYER_FRAMES 3 //Antal frames i spritesheet
#define SPLASH_FRAMES 14
#define TIME_DELAY 100

struct loadMedia {
	SDL_Texture* flyTrapTex;
	SDL_Texture* flyTex;
	SDL_Texture* flySplashTex;
	SDL_Surface* flyTrapSurface;
	SDL_Surface* flySurface;
	SDL_Surface* flySplashSurface;
	SDL_Rect playerSprites[PLAYER_FRAMES];
	SDL_Rect splashSprites[SPLASH_FRAMES];
	SDL_Texture* backgroundTex;
	SDL_Surface* backgroundSurface;
};
typedef struct loadMedia* LoadMedia;

LoadMedia loadMedia(SDL_Renderer* renderer, bool* running);

#endif // !LOADMEDIA_H