#ifndef game_engine_h
#define game_engine_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "player.h"
#include "obstacle.h"
#include "Network.h"
#include "world.h"
#include "loadMedia.h"
#include "LoadMenu.h"
#include "input.h"
#include "constants.h"

//params a renderer, screen width, screen height
bool startGame(SDL_Renderer* renderer, int w, int h, char playerName[], char playerIp[], LoadMedia media, Fonts fonts, Game_State current, UDP_Client_Config setup, Game_Route *aGameRoute, Store storeStatus);
bool startClientGame(SDL_Renderer* renderer, int w, int h, char playerName[], char playerIp[], LoadMedia media, Fonts fonts, Game_State current, UDP_Client_Config setup, Game_Route *aGameRoute, Store storeStatus);


#endif /*game_engine_h*/