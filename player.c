#include "player.h"

static int playerHeight = 64;
static int playerWidth = 100;

Player createPlayer(int x, int y) {
	Player aPlayer = malloc(sizeof(struct playerType));
	aPlayer->x = x;
	aPlayer->y = y;
	aPlayer->h = playerHeight;
	aPlayer->w = playerWidth;
	return aPlayer;
}


int getPlayerPositionX(Player aPlayer) {
	return aPlayer->x;
}

int getPlayerPositionY(Player aPlayer) {
	return aPlayer->y;
}

int getPlayerWidth(Player aPlayer) {
	return aPlayer->w;
}

int getPlayerHeight(Player aPlayer) {
	return aPlayer->h;
}

