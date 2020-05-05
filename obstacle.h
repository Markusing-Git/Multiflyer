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

typedef struct obstacle_type* Obstacle;

//param screen width, screen height, creates obstacle and two SDL_Rects with random opening.
PUBLIC Obstacle createObstacle(int screenWidth, int screenHeight);

//initates random gen for opening
PUBLIC void initRandomGeneratior();

//creates a random opening for Obstacle
PUBLIC void rndOpening(Obstacle aObs, int screenHeight);

//param a obstacle, true for top false for bot, x:y:w:h for cordinates
PUBLIC int getObstacleValue(Obstacle aObs, bool topOrBot, char value);

//param a obstacle, true for top false for bot
PUBLIC SDL_Rect getRectfromObstacle(Obstacle aObs, bool topOrBot);

//param a obstacle and rect, true for top false for bot
PUBLIC void setRectfromObstacle(Obstacle aObs, SDL_Rect aRect, bool topOrBot);

//param a obstacle, returns next obstacle in the list
PUBLIC Obstacle getNextObsFromList(Obstacle aObs);

//param obstacle list head, screenwidth, screenHeigt. Adds new obstacle to the list,
PUBLIC void newObstacle(Obstacle head, int screenWidth, int screenHeight);

//param obstacle list head, advances obstacles from right to left.
PUBLIC void obsteclesTick(Obstacle head);

//param obstacle list head, a raenderer, a texture. renders obstacles to the screen.
PUBLIC void renderObstacles(Obstacle head, SDL_Renderer* renderer, SDL_Texture* texture);

//Checks if player has colided with obstacle
PUBLIC void obstacleCollision(SDL_Rect* aPlayerPos, Player aPlayer, Obstacle head);

//creates a client obstacle
PUBLIC Obstacle createClientObstacle(SDL_Rect top, SDL_Rect bottom);

//Creates a client obstacle list
PUBLIC void newClientObstacle(Obstacle receivedObstacle, Obstacle head);

//Checks if player has passed an obstacle
PUBLIC void checkIfPassed(SDL_Rect* aPlayerPos, Player aPlayer, Obstacle head);

#endif /*Obstacle_h*/