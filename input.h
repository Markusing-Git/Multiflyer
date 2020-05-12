#ifndef input_h
#define input_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "player.h"
#include "constants.h"

//change player speeds here
#define PLAYER_VELOCITY 5
#define GLIDING_VELOCITY 2
#define HOVER_VELOCITY 1
#define HOVER_DISTANCE 15
#define PUSH_VELOCITY 2

typedef struct input_type* Inputs;

//initates input variables
Inputs initInputs(void);

//polling input events from keyboard
void pollInputEvents(SDL_Event* aEvent, bool* aRunning, Player aPlayer, Inputs aInput, Game_Route *aGameRoute, bool* space);

//Adding inputs to player position
void uppdateInputs(Player aPlayer, Inputs aInput);

//Calls movePlayer-function based on the pushAngle in case of a push
void pushPlayer(Player aPlayer, int pushAngle);

//frees input structure from heap
void QuitInput(Inputs aInput);

#endif /*input_h*/

