#ifndef LoadMenu_h
#define LoadMenu_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

#define NAME_LENGTH 50
#define IP_LENGTH 50

int LoadMenu(SDL_Renderer* renderer, SDL_Window* window, int w, int h, bool* hostOrClient, char name[], char ip[]);
void getHostOrClient(SDL_Renderer* renderer, bool* hostOrClient);
void enterName(SDL_Renderer* renderer, char name[]);
void enterIp(SDL_Renderer* renderer, char ip[]);

#endif 