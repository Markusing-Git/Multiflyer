#ifndef player_h
#define player_h

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

//en ADT f�r att skapa en spelare
//getPlayerPositionX returnerar spelarens x cordinat
//getPlayerPositionY returnerar spelarens y cordinat
//drawPlayer ritar ut spelaren p� sk�rmen (Kanske borde tillh�ra annan source fil t.ex en som heter game??)

typedef struct playerType *Player;

int getPlayerPositionX(Player aPlayer);
int getPlayerPositionY(Player aPlayer);
Player createPlayer(int x, int y);
void drawPlayer(SDL_Surface* screen, int x, int y);

#endif /* player_h */