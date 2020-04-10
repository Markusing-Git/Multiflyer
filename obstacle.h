#ifndef Obstacle_h
#define Obstacle_h

#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include "player.h"

/*Obstacles are created in list form by first initating a pointer to typ Obstacle and initiating head with createObstacle.
  creatiotion can be automated with help of a delay function in game engine,
  destroy each obstacle that reaches out of screen with destroyObstacle()*/

struct obstacle_type {
    SDL_Rect top;
    SDL_Rect bottom;
    struct obstacle_type* next;
};

typedef struct obstacle_type *Obstacle;

//param screen width, screen height, creates obstacle and two SDL_Rects with random opening.
Obstacle createObstacle(int screenWidth, int screenHeight);

//param a obstacle, true for top false for bot, x:y:w:h for cordinates
int getObstacleValue(Obstacle aObs, bool topOrBot, char value);

//param a obstacle, true for top false for bot
SDL_Rect getRectfromObstacle(Obstacle aObs, bool topOrBot);

//param a obstacle front of list
bool destroyObstacle(Obstacle head);

//initates random generator seed
void initRandomGeneratior(void);

//param a abstacle, screen height, creates random opening in obstacles.
void rndOpening(Obstacle aObs, int screenHeight);

//param obstacle list head, screenwidth, screenHeigt. Adds new obstacle to the list,
void newObstacle(Obstacle head, int screenWidth, int screenHeight);

//param obstacle list head, advances obstacles from right to left.
void obsteclesTick(Obstacle head);

//param obstacle list head, a raenderer, a texture. renders obstacles to the screen.
void renderObstacles(Obstacle head, SDL_Renderer* renderer, SDL_Texture* texture);

//Checks if player has colided with obstacle
void obstacleCollision(SDL_Rect* aPlayerPos, Player aPlayer, Obstacle head);

//Creates a client obstacle list
void newClientObstacle(Obstacle receivedObstacle, Obstacle head);

#endif /*Obstacle_h*/