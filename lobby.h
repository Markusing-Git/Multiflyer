#ifndef lobby_h
#define lobby_h

#define _CRT_SECURE_NO_DEPRECATE
#define MAX_PLAYERS 3
#define NAME_LENGTH 50
#define IP_LENGTH 50
#define TEXTS 9
#define PUBLIC
#define PRIVATE static

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include "player.h"

//starts a client based lobby
void clientLobby(SDL_Renderer* renderer);

//starts a host based lobby
int hostLobby(SDL_Renderer* renderer);

#endif /*lobby_h*/