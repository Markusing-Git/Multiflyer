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

int LoadMenu(SDL_Renderer* renderer, SDL_Window* window, int w, int h, bool* hostOrClient, char name[], char ip[], LoadMedia media);
void getHostOrClient(SDL_Renderer* renderer, bool* hostOrClient);
void enterName(SDL_Renderer* renderer, char name[]);
void enterIp(SDL_Renderer* renderer, char ip[]);
void control(SDL_Renderer* renderer);

#endif 