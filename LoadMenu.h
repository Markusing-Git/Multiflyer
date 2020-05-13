#ifndef LoadMenu_h
#define LoadMenu_h

#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <SDL2/SDL_mixer.h>
#include "lobby.h"
#include "loadMedia.h"
#include "Network.h"
#include "constants.h"

#define NUM_MENU 6
#define OPTIONS 5
#define VOL_SURFACES 10
#define VOL_DUPLICATES 2

struct audio
{
    bool done;
    bool renderText;

    int bMusicVolume;
    int sEffectsVolume;
    char headLine[NAME_LENGTH];
    char bMusic[NAME_LENGTH];
    char sEffects[NAME_LENGTH];
    char lines[VOL_DUPLICATES][NAME_LENGTH];
    char changing_name[NUM_MENU][NAME_LENGTH]; //Text for "Back to menu" button and "+/-""

    SDL_Surface* surfaces[VOL_SURFACES];
    SDL_Texture* headLine_Tex;
    SDL_Texture* bMusic_Tex;
    SDL_Texture* sEffects_Tex;
    SDL_Texture* lines_Tex[VOL_DUPLICATES];
    SDL_Texture* changing_Tex[NUM_MENU]; //Texture for "Back to menu" button and "+/-""

    SDL_Rect headLine_Rect;
    SDL_Rect bMusic_Rect;
    SDL_Rect sEffects_Rect;
	SDL_Rect lines_Rect[VOL_DUPLICATES];
    SDL_Rect changing_Rect[NUM_MENU]; //Rect for "Back to menu" button and "+/-""

    SDL_Color white;
    SDL_Color green;
};
typedef struct audio* Audio;

int LoadMenu(SDL_Renderer* renderer, SDL_Window* window, int w, int h, char name[], char ip[],
	LoadMedia media, Fonts fonts, Game_State current, UDP_Client_Config setup, Game_Route *aGameRoute, Audio settings);
void getHostOrClient(SDL_Renderer* renderer, LoadMedia media, Game_Route *aGameRoute);
void enterName(SDL_Renderer* renderer, LoadMedia media, Fonts fonts, char name[]);
void enterIp(SDL_Renderer* renderer, LoadMedia media, Fonts fonts, char ip[]);
void control(SDL_Renderer* renderer, LoadMedia media, Game_Route *aGameRoute);
void initSettings(Audio settings);
void volume(SDL_Renderer* renderer, LoadMedia media, Fonts fonts, Audio settings);
//opens a scoreboard and with alternatives to return to menu or start again
void openScoreBoard(SDL_Renderer* renderer, LoadMedia media, Fonts fonts, Game_State current, Game_Route *aGameRoute);

#endif 