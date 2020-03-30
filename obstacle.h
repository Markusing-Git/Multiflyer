#ifndef obstacle_h
#define obstacle_h

#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <stdlib.h>

#define OBSTACLE_TOP 0

typedef struct obstacle_type* Obstacle;

//createObstacleTop skapar den �vredelen av ett hinder, tar screen width, detta f�r att g�ra den dynamisk till
//hur stor sk�rmen kommer att vara.
Obstacle createObstacleTop(int screenWidth);

//createObstacleBottom skapar den nedre delen av ett hinder, tar ett tophinder som parameter samt h�jden p� skr�rmen.
Obstacle createObstacleBottom(Obstacle topObstacle, int screenHeight);

int getObstacleX(Obstacle aObs);
int getObstacleY(Obstacle aObs);
int getObstacleWidth(Obstacle aObs);
int getObstacleHeight(Obstacle aObs);

//raderar hinder fr�n heap.
void destroyObstacle(Obstacle aObs);

//tick flyttar hinder �t v�nster m�ste anv�ndas p� b�de top och bottom hinder. g�r att �ndra hastigheten.
void obstacleTick(Obstacle aObs);

//random generator
void initRandomGeneratior(void);
int rndNumber(void);

#endif /*obstacle_h*/