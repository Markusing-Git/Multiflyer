#ifndef obstacle_h
#define obstacle_h

#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <stdlib.h>

#define OBSTACLE_TOP 0

typedef struct obstacle_type* Obstacle;

//createObstacleTop skapar den övredelen av ett hinder, tar screen width, detta för att göra den dynamisk till
//hur stor skärmen kommer att vara.
Obstacle createObstacleTop(int screenWidth);

//createObstacleBottom skapar den nedre delen av ett hinder, tar ett tophinder som parameter samt höjden på skrärmen.
Obstacle createObstacleBottom(Obstacle topObstacle, int screenHeight);

int getObstacleX(Obstacle aObs);
int getObstacleY(Obstacle aObs);
int getObstacleWidth(Obstacle aObs);
int getObstacleHeight(Obstacle aObs);

//raderar hinder från heap.
void destroyObstacle(Obstacle aObs);

//tick flyttar hinder åt vänster måste användas på både top och bottom hinder. går att ändra hastigheten.
void obstacleTick(Obstacle aObs);

//random generator
void initRandomGeneratior(void);
int rndNumber(void);

#endif /*obstacle_h*/