#include "Obstacle.h"

struct obstacle_type {
    SDL_Rect top;
    SDL_Rect bottom;
    struct obstacle_type* next;
};


Obstacle createObstacle(int screenWidth, int screenHeight) {
    Obstacle aObs = malloc(sizeof(struct obstacle_type));
    aObs->top.x = screenWidth;
    aObs->top.w = 65;
    aObs->top.h = screenHeight/2;

    aObs->bottom.x = screenWidth;
    aObs->bottom.w = 65;
    aObs->bottom.h = screenHeight/2;
    aObs->next = NULL;

    rndOpening(aObs, screenHeight);
    return aObs;
}

//param a obstacle, true for top false for bot, x:y:w:h for cordinates
int getObstacleValue(Obstacle aObs, bool topOrBot, char value) {
    if (topOrBot) {
        switch (value) {
        case 'x':
            return aObs->top.x;
        case 'y':
            return aObs->top.y;
        case 'w':
            return aObs->top.w;
        case 'h':
            return aObs->top.h;
        default:
            printf("Error: invalid coordinates");
            return 0;
        }
    }
    else {
        switch (value) {
        case 'x':
            return aObs->bottom.x;
        case 'y':
            return aObs->bottom.y;
        case 'w':
            return aObs->bottom.w;
        case 'h':
            return aObs->bottom.h;
        default:
            printf("Error: invalid coordinates");
            return 0;
        }
    }
}

SDL_Rect getRectfromObstacle(Obstacle aObs, bool topOrBot) {
    if (topOrBot) {
        return aObs->top;
    }
    else {
        return aObs->bottom;
    }
}

void initRandomGeneratior(void) {
    srand((unsigned)time(NULL));
}

void rndOpening(Obstacle aObs, int screenHeight) {

    int obsCenter = screenHeight / 2;
    int opening = screenHeight / 7;
    int pipe1 = obsCenter - screenHeight / 8;
    int pipe2 = obsCenter + screenHeight / 8;

    int nbr = (rand() % 3 - 0) + 1;

    switch (nbr) {
    case 1:
        aObs->top.y = pipe1 - opening - aObs->top.h;
        aObs->bottom.y = pipe1 + opening;
        break;
    case 2:
        aObs->top.y = obsCenter - opening - aObs->top.h;
        aObs->bottom.y = obsCenter + opening;
        break;
    case 3:
        aObs->top.y = pipe2 - opening - aObs->top.h;
        aObs->bottom.y = pipe2 + opening;
        break;
    default:
        break;
    }
}


bool  destroyObstacle(Obstacle head) {
    Obstacle current,  prev;
    current = head;
    current = current->next;
    prev = current;
    while (current != NULL) {
        if (current->top.x + current->top.w == 0) {
            prev->next = current->next;
            free(current);
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}


void newObstacle(Obstacle head, int screenWidth, int screenHeight) {
    Obstacle newNode;

    newNode = createObstacle(screenWidth, screenHeight);

    newNode->next = head->next;
    head->next = newNode;
}

void obsteclesTick(Obstacle head) {
    Obstacle obs;
    obs = head;
    obs = obs->next;
    while (obs != NULL) {
        obs->top.x--;
        obs->bottom.x--;
        obs = obs->next;
    }
}

void renderObstacles(Obstacle head, SDL_Renderer* renderer, SDL_Texture* texture) {
    Obstacle obs;
    obs = head;
    obs = obs->next;
    while (obs != NULL) {
        SDL_RenderCopyEx(renderer, texture, NULL, &obs->top, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(renderer, texture, NULL, &obs->bottom, 0, NULL, SDL_FLIP_VERTICAL);
        obs = obs->next;
    }
}