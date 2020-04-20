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


void pollInputEvents(SDL_Event* aEvent, bool* aRunning, Player aPlayer, Inputs aInput) {
    while (SDL_PollEvent(aEvent))
    {
        switch (aEvent->type)
        {
        case SDL_QUIT:
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

    if (aInput->pattern == 0)
        aInput->pattern = ((rand() % 3 - 0) + 1);

    switch (aInput->pattern) {

        //pattern 1
    case 1:
        if (aInput->state >= 0 && aInput->state < 5) {
            movePlayerUp(aPlayer, speed);
            aInput->state++;
        }
        else if (aInput->state >= 5 && aInput->state < 10) {
            movePlayerUp(aPlayer, speed);
            aInput->state++;
        }
        else if (aInput->state >= 10 && aInput->state < 15) {
            movePlayerRight(aPlayer, speed);
            movePlayerDown(aPlayer, speed);
            aInput->state++;
        }
        else if (aInput->state >= 15 && aInput->state < 20) {
            movePlayerRight(aPlayer, speed);
            movePlayerDown(aPlayer, speed);
            aInput->state++;
        }
        else if (aInput->state >= 20) {
            aInput->state = 0;
            aInput->pattern = 0;
        }
        break;

        //pattern 2
    case 2:
        if (aInput->state >= 0 && aInput->state < 5) {
            movePlayerLeft(aPlayer, speed);
            movePlayerDown(aPlayer, speed);
            aInput->state++;
        }
        else if (aInput->state >= 5 && aInput->state < 10) {
            movePlayerLeft(aPlayer, speed);
            movePlayerDown(aPlayer, speed);
            aInput->state++;
        }
        else if (aInput->state >= 10 && aInput->state < 15) {
            movePlayerDown(aPlayer, speed);
            aInput->state++;
        }
        else if (aInput->state >= 15 && aInput->state < 20) {
            movePlayerDown(aPlayer, speed);
            aInput->state++;
        }
        else if (aInput->state >= 20) {
            aInput->state = 0;
            aInput->pattern = 0;
        }
        break;

        //pattern 3
    case 3:
        if (aInput->state >= 0 && aInput->state < 5) {
            movePlayerRight(aPlayer, speed);
            movePlayerUp(aPlayer, speed);
            aInput->state++;
        }
        else if (aInput->state >= 5 && aInput->state < 10) {
            movePlayerRight(aPlayer, speed);
            movePlayerUp(aPlayer, speed);
            aInput->state++;
        }
        else if (aInput->state >= 10 && aInput->state < 15) {
            movePlayerLeft(aPlayer, speed);
            movePlayerUp(aPlayer, speed);
            aInput->state++;
        }
        else if (aInput->state >= 15 && aInput->state < 20) {
            movePlayerLeft(aPlayer, speed);
            movePlayerUp(aPlayer, speed);
            aInput->state++;
        }
        else if (aInput->state >= 20) {
            aInput->state = 0;
            aInput->pattern = 0;
        }
        break;
    }
}

void QuitInput(Inputs aInput) {
    free(aInput);
}