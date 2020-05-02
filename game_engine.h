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
#include "input.h"
#include "constants.h"

//params a renderer, screen width, screen height
bool startGame(SDL_Renderer* renderer, int w, int h, char playerName[], char playerIp[], LoadMedia media, Game_State current, UDP_Client_Config setup);
bool startClientGame(SDL_Renderer* renderer, int w, int h, char playerName[], char playerIp[], LoadMedia media, Game_State current, UDP_Client_Config setup);

#endif /*game_engine_h*/