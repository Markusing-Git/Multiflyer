#ifndef LOADMEDIA_H
#define LOADMEDIA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "constants.h"

struct loadMedia {
	SDL_Texture* flyTrapTex;
	SDL_Texture* flyTex;
	SDL_Texture* flySplashTex;
	SDL_Texture* backgroundTex;
	SDL_Texture* scoreBoardTexture;
	SDL_Texture* menuBackgroundTexture;
	SDL_Texture* controlsTexture;
	SDL_Texture* hostButtonTexture;
	SDL_Texture* clientButtonTexture;
	SDL_Texture* textboxTexture;
	SDL_Texture* scoreBackgroundTex;
	SDL_Texture* scoreTex;
	SDL_Texture* storeTex;
	SDL_Texture* PowerUpTex[3];
	SDL_Texture* heartTex[2];
	SDL_Texture* immunityTex;
	SDL_Texture* coinTex;

	SDL_Surface* flyTrapSurface;
	SDL_Surface* flySurface;
	SDL_Surface* flySplashSurface;
	SDL_Surface* backgroundSurface;
	SDL_Surface* scoreboardSurface;
	SDL_Surface* menuBackgroundSurface;
	SDL_Surface* controlsSurface;
	SDL_Surface* hostButtonSurface;
	SDL_Surface* clientButtonSurface;
	SDL_Surface* textboxSurface;
	SDL_Surface* scoreBackgroundSurface;
	SDL_Surface* score;
	SDL_Surface* storeSurface;
	SDL_Surface* powerUpSurface[3];
	SDL_Surface* heartSurface[2];
	SDL_Surface* immunitySurface;
	SDL_Surface* coinSurface;

	SDL_Rect startFlyBlue[PLAYER_FRAMES];
	SDL_Rect startFlyGreen[PLAYER_FRAMES];
	SDL_Rect startFlyRed[PLAYER_FRAMES];
	SDL_Rect startFlyYellow[PLAYER_FRAMES];
	SDL_Rect splashSprites[SPLASH_FRAMES];
	SDL_Rect scrollingBackground[2];
	SDL_Rect scoreBackgroundRect;
	SDL_Rect scoreRect;
	SDL_Rect heartRect[2];
	SDL_Rect immunitySprites[IMMUNITY_FRAMES];
	SDL_Rect immunityRect;
	SDL_Rect glowRect;
	SDL_Rect coinSprites[COIN_FRAMES];
	SDL_Rect storeRect;

	Mix_Chunk* flyingNoise;
	Mix_Chunk* electricShock;
	Mix_Music* menuMusic;
};
typedef struct loadMedia* LoadMedia;


//struct for fonts
struct loadFonts {
	TTF_Font* magical_45;
	TTF_Font* magical_36;
	TTF_Font* cuvert_24;
	TTF_Font* cuvert_28;
	TTF_Font* cuvert_48;
	TTF_Font* cuvert_60;
	TTF_Font* ka1_60;
	TTF_Font* scoreFont_40;
	TTF_Font* scoreFont_24;
};

typedef struct loadFonts* Fonts;

//loads all media, add any graphics here. For use create a empty Loadmedia structere and use this function to return content.
LoadMedia loadMedia(SDL_Renderer* renderer, bool* running);

//Loads all fonts, add any fonts. For use create a empty Loadmedia structere and use this function to return content.
Fonts loadFonts(void);

//deletes all created fonts and quits ttf
void closeFonts(Fonts mediaFonts);

#endif // !LOADMEDIA_H