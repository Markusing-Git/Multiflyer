#ifndef input_h
#define input_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "player.h"
#include "constants.h"
#include "Network.h"

//change player speeds here
#define PLAYER_VELOCITY 5
#define GLIDING_VELOCITY 2
#define HOVER_VELOCITY 1
#define HOVER_DISTANCE 15
#define PUSH_VELOCITY 5

typedef struct input_type* Inputs;

//initates input variables
PUBLIC Inputs initInputs(void);

//polling input events from keyboard
PUBLIC void pollInputEvents(SDL_Event* aEvent, bool* aRunning, Player aPlayer, Inputs aInput, Game_Route *aGameRoute, bool* space);

//Adding inputs to player position
PUBLIC void uppdateInputs(Player aPlayer, Inputs aInput, Game_State current);

//void attack(int nrOfPlayers, int localPlayerNr, int* pushAngle[], Player players[], int* change_flag, Uint32* spaceDelay, bool space, int* nrOfPushes);

PUBLIC void playerAttack(Game_State current, Player players[], Uint32* spaceDelay, int* nrOfPushes, bool space);

//frees input structure from heap
PUBLIC void QuitInput(Inputs aInput);

#endif /*input_h*/

