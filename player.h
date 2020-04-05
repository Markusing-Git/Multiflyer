#ifndef player_h
#define player_h

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

//en ADT för att skapa en spelare
//getPlayerPositionX returnerar spelarens x cordinat
//getPlayerPositionY returnerar spelarens y cordinat
//drawPlayer ritar ut spelaren på skärmen (Kanske borde tillhöra annan source fil t.ex en som heter game??)

struct playerType {
	int x;
	int y;
	int h;
	int w;
};

typedef struct playerType *Player;

int getPlayerPositionX(Player aPlayer);
int getPlayerPositionY(Player aPlayer);
int getPlayerWidth(Player aPlayer);
int getPlayerHeight(Player aPlayer);
Player createPlayer(int x, int y);

#endif /* player_h */