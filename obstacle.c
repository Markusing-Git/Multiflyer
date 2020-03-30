#include "obstacle.h"

struct obstacle_type {
    int x;
    int y;
    int width;
    int height;
};

Obstacle createObstacleTop(int screenWidth) {
    Obstacle aObs = malloc(sizeof(struct obstacle_type));
    aObs->width = screenWidth / 15;
    aObs->height = rndNumber();
    aObs->x = (screenWidth - aObs->width);
    aObs->y = OBSTACLE_TOP;
    return aObs;
}

//param höjden på obstacl1 och adderar en offset
Obstacle createObstacleBottom(Obstacle topObstacle, int screenHeight) {
    Obstacle aObs = malloc(sizeof(struct obstacle_type));
    aObs->width = topObstacle->width;
    aObs->height = screenHeight;
    aObs->x = topObstacle->x;
    aObs->y = (topObstacle->height + (screenHeight / 5));
    return aObs;
}

int getObstacleX(Obstacle aObs) {
    return aObs->x;
}

int getObstacleY(Obstacle aObs) {
    return aObs->y;
}

int getObstacleWidth(Obstacle aObs) {
    return aObs->width;
}

int getObstacleHeight(Obstacle aObs) {
    return aObs->height;
}

void initRandomGeneratior(void) {
    srand((unsigned)time(NULL));
}

int rndNumber(void) {

    initRandomGeneratior();
    int nbr;

    nbr = (rand() % (400 - 200 + 1)) + 200;
    return nbr;
}

void destroyObstacle(Obstacle aObs) {
    free(aObs);
}

void obstacleTick(Obstacle aObs) {
    aObs->x--;
}