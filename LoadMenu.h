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

#define NUM_MENU 4

int LoadMenu(SDL_Renderer* renderer, SDL_Window* window, int w, int h, char name[], char ip[],
	LoadMedia media, Fonts fonts, Game_State current, UDP_Client_Config setup, Game_Route *aGameRoute);
void getHostOrClient(SDL_Renderer* renderer, LoadMedia media, Game_Route *aGameRoute);
void enterName(SDL_Renderer* renderer, LoadMedia media, Fonts fonts, char name[]);
void enterIp(SDL_Renderer* renderer, LoadMedia media, Fonts fonts, char ip[]);
void control(SDL_Renderer* renderer, LoadMedia media);

//opens a scoreboard and with alternatives to return to menu or start again
void openScoreBoard(SDL_Renderer* renderer, LoadMedia media, Fonts fonts, Game_State current, Game_Route *aGameRoute);

#endif 