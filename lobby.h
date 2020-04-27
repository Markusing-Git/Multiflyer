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



typedef struct lobby_type* Lobby;

PUBLIC int hostLobby(SDL_Renderer* renderer, char playerName[]);
PUBLIC void clientLobby(SDL_Renderer* renderer, char playerName[], char playerIp[]);

#endif /*lobby_h*/