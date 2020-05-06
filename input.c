#include "input.h"

static void hoverFly(Player aPlayer, Inputs aInput);

struct input_type {
    //keyboard
    bool released[4];
    int releasecount[4];
    bool push[4];
    int state;
    int pattern;
    int speed;
    int glideEffect;
};

Inputs initInputs(void) {
    Inputs aInput = malloc(sizeof(struct input_type));

    //movement inputs
    for (int i = 0; i < 4; i++)
        aInput->released[i] = false;

    for (int i = 0; i < 4; i++)
        aInput->releasecount[i] = 10;

    for (int i = 0; i < 4; i++)
        aInput->push[i] = false;

    //hovering properties
    aInput->state = 0;
    aInput->pattern = 0;

    //speeds
    aInput->speed = PLAYER_VELOCITY;
    aInput->glideEffect = GLIDING_VELOCITY;

    return aInput;
}


void pollInputEvents(SDL_Event* aEvent, bool* aRunning, Player aPlayer, Inputs aInput, Game_Route *aGameRoute) {
    while (SDL_PollEvent(aEvent))
    {
        switch (aEvent->type)
        {
        case SDL_QUIT:
            *aGameRoute = quitRoute;
            *aRunning = false;
            break;
        case SDL_KEYDOWN:
            if (getPlayerStatus(aPlayer) == true) {
                switch (aEvent->key.keysym.sym)
                {
                case SDLK_UP:
                    aInput->push[0] = true;
                    break;
                case SDLK_DOWN:
                    aInput->push[1] = true;
                    break;
                case SDLK_LEFT:
                    aInput->push[2] = true;
                    break;
                case SDLK_RIGHT:
                    aInput->push[3] = true;
                    break;
                case SDLK_ESCAPE:
                    *aRunning = false;
                    break;
                }
            }
            break;
        case SDL_KEYUP: //Släpper knappen
            if (getPlayerStatus(aPlayer) == true) {
                switch (aEvent->key.keysym.sym)
                {
                case SDLK_UP:
                    aInput->push[0] = false;
                    aInput->released[0] = true;
                    break;
                case SDLK_DOWN:
                    aInput->push[1] = false;
                    aInput->released[1] = true;
                    break;
                case SDLK_LEFT:
                    aInput->push[2] = false;
                    aInput->released[2] = true;
                    break;
                case SDLK_RIGHT:
                    aInput->push[3] = false;
                    aInput->released[3] = true;
                    break;
                }
            }
            break;
        }
    }
}


//************************* MOVES PLAYER AND SETS PLAYER SPEED *****************************
void uppdateInputs(Player aPLayer, Inputs aInput) {

    if (getPlayerStatus(aPLayer) == true) {
        if (aInput->push[0] || aInput->released[0])
        {
            if (!aInput->released[0]) {
                movePlayerUp(aPLayer, aInput->speed);
            }
            else {
                movePlayerUp(aPLayer, aInput->glideEffect);
                aInput->releasecount[0]--;
                if (aInput->releasecount[0] <= 0) {
                    aInput->released[0] = false;
                    aInput->releasecount[0] = 10;
                }
            }

        }
        if (aInput->push[1] || aInput->released[1])
        {
            if (!aInput->released[1]) {
                movePlayerDown(aPLayer, aInput->speed);
            }
            else {
                movePlayerDown(aPLayer, aInput->glideEffect);
                aInput->releasecount[1]--;
                if (aInput->releasecount[1] <= 0) {
                    aInput->released[1] = false;
                    aInput->releasecount[1] = 10;
                }
            }
        }
        if (aInput->push[2] || aInput->released[2])
        {
            if (!aInput->released[2]) {
                movePlayerLeft(aPLayer, aInput->speed);
            }
            else {
                movePlayerLeft(aPLayer, aInput->glideEffect);
                aInput->releasecount[2]--;
                if (aInput->releasecount[2] <= 0) {
                    aInput->released[2] = false;
                    aInput->releasecount[2] = 10;
                }
            }
        }
        if (aInput->push[3] || aInput->released[3])
        {
            if (!aInput->released[3]) {
                movePlayerRight(aPLayer, aInput->speed);
            }
            else {
                movePlayerRight(aPLayer, aInput->glideEffect);
                aInput->releasecount[3]--;
                if (aInput->releasecount[3] <= 0) {
                    aInput->released[3] = false;
                    aInput->releasecount[3] = 10;
                }
            }
        }
        hoverFly(aPLayer, aInput);
    }
}


static void hoverFly(Player aPlayer, Inputs aInput) {

    int speed = HOVER_VELOCITY;
    int distance = HOVER_DISTANCE;

    if (aInput->pattern == 0)
        aInput->pattern = ((rand() % 8 - 0) + 1);

    switch (aInput->pattern) {
    case 1:
        movePlayerUp(aPlayer, speed);
        break;
    case 2:
        movePlayerUp(aPlayer, speed);
        movePlayerRight(aPlayer, speed);
        break;
    case 3:
        movePlayerRight(aPlayer, speed);
        break;
    case 4:
        movePlayerRight(aPlayer, speed);
        movePlayerDown(aPlayer, speed);
        break;
    case 5:
        movePlayerDown(aPlayer, speed);
        break;
    case 6:
        movePlayerDown(aPlayer, speed);
        movePlayerLeft(aPlayer, speed);
        break;
    case 7:
        movePlayerLeft(aPlayer, speed);
        break;
    case 8:
        movePlayerUp(aPlayer, speed);
        movePlayerLeft(aPlayer, speed);
        break;
    }
    aInput->state++;
    if (aInput->state == distance) {
        aInput->pattern = 0;
        aInput->state = 0;
    }
}


void QuitInput(Inputs aInput) {
    free(aInput);
}