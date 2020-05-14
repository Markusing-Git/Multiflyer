#include "obstacle.h"


struct obstacle_type {
    SDL_Rect top;
    SDL_Rect bottom;
    int x1, y1, x2, y2;
    bool passed;
    struct obstacle_type* next;
};

PRIVATE bool destroyObstacle(Obstacle head);


PUBLIC Obstacle createObstacle(int screenWidth, int screenHeight) {
    Obstacle aObs = malloc(sizeof(struct obstacle_type));
    aObs->top.x = screenWidth;
    aObs->top.w = 65;
    aObs->top.h = screenHeight / 2;

    aObs->bottom.x = screenWidth;
    aObs->bottom.w = 65;
    aObs->bottom.h = screenHeight / 2;
    aObs->next = NULL;

    aObs->x1 = screenWidth + 130;
    aObs->y1 = 0;
    aObs->x2 = screenWidth + 130;
    aObs->y2 = screenHeight;

    aObs->passed = false;

    rndOpening(aObs, screenHeight);
    return aObs;
}

PUBLIC Obstacle createClientObstacle(SDL_Rect top, SDL_Rect bottom) {
    Obstacle aObs = malloc(sizeof(struct obstacle_type));
    aObs->top = top;
    aObs->bottom = bottom;
    aObs->next = NULL;
    aObs->passed = false;

    aObs->x1 = WINDOW_WIDTH + 130;
    aObs->y1 = 0;
    aObs->x2 = WINDOW_WIDTH + 130;
    aObs->y2 = WINDOW_HEIGHT;

    return aObs;
}

//param a obstacle, true for top false for bot, x:y:w:h for cordinates
PUBLIC int getObstacleValue(Obstacle aObs, bool topOrBot, char value) {
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

PUBLIC SDL_Rect getRectfromObstacle(Obstacle aObs, bool topOrBot) {
    if (topOrBot) {
        return aObs->top;
    }
    else {
        return aObs->bottom;
    }
}

PUBLIC void setRectfromObstacle(Obstacle aObs, SDL_Rect aRect, bool topOrBot) {
    if (topOrBot) {
        aObs->top = aRect;
    }
    else {
        aObs->bottom = aRect;
    }
}

PUBLIC Obstacle getNextObsFromList(Obstacle aObs) {
    return aObs->next;
}

PUBLIC void initRandomGeneratior(void) {
    srand((unsigned)time(NULL));
}

PUBLIC void rndOpening(Obstacle aObs, int screenHeight) {

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


PRIVATE bool destroyObstacle(Obstacle head) {
    Obstacle current, prev;
    current = head;
    current = current->next;
    prev = current;
    while (current != NULL) {
        if (current->top.x + current->top.w <= 0) {
            prev->next = current->next;
            free(current);
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}


PUBLIC void newObstacle(Obstacle head, int screenWidth, int screenHeight) {
    Obstacle newNode;

    newNode = createObstacle(screenWidth, screenHeight);

    newNode->next = head->next;
    head->next = newNode;
}

PUBLIC void newClientObstacle(Obstacle receivedObstacle, Obstacle head) {
    Obstacle newNode;
   

    newNode = receivedObstacle;

    newNode->next = head->next;
    head->next = newNode;
}

PUBLIC void obsteclesTick(Obstacle head) {
    Obstacle obs;
    obs = head;
    obs = obs->next;
    while (obs != NULL) {
        obs->top.x -= 2;
        obs->bottom.x -= 2;
        obs->x1 -= 2;
        obs->x2 -= 2;
        obs = obs->next;
    }
    destroyObstacle(head);
}

PUBLIC void renderObstacles(Obstacle head, SDL_Renderer* renderer, SDL_Texture* texture) {
    Obstacle obs;
    obs = head;
    obs = obs->next;
    while (obs != NULL) {
        SDL_RenderCopyEx(renderer, texture, NULL, &obs->top, 0, NULL, SDL_FLIP_VERTICAL);
        SDL_RenderCopyEx(renderer, texture, NULL, &obs->bottom, 0, NULL, SDL_FLIP_NONE);
        obs = obs->next;
    }
}

PUBLIC void obstacleCollision(SDL_Rect* aPlayerPos, Player aPlayer, Obstacle head) {
    /*SDL_Rect pixelRect;
    pixelRect.x = aPlayerPos->x + 5;
    pixelRect.y = aPlayerPos->y + 23;
    pixelRect.w = aPlayerPos->w - 10;
    pixelRect.h = aPlayerPos->h - 30;
    Obstacle obs;
    obs = head;
    obs = obs->next;
    while (obs != NULL) {
        if (SDL_HasIntersection(&pixelRect, &obs->top) || SDL_HasIntersection(&pixelRect, &obs->bottom)) {
            if(!getPlayerImmunity(aPlayer))
                setPlayerStatus(aPlayer, false);
        }
        obs = obs->next;
    }*/
}

PUBLIC void checkIfPassed(SDL_Rect* aPlayerPos, Player aPlayer, Obstacle head) {
    SDL_Rect pixelRect;
    pixelRect.x = aPlayerPos->x;
    pixelRect.y = aPlayerPos->y;
    pixelRect.w = aPlayerPos->w;
    pixelRect.h = aPlayerPos->h;
    Obstacle obs;
    obs = head;
    obs = obs->next;

    while (obs != NULL) {
        if (SDL_IntersectRectAndLine(&pixelRect, &obs->x1, &obs->y1, &obs->x2, &obs->y2)) {
            if (!obs->passed) {
                addScore(aPlayer);
                obs->passed = true;
            }
        }
        obs = obs->next;
    }
}