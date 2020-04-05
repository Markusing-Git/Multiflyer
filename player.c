#include "player.h"

#define PLAYER_HEIGHT 15
#define PLAYER_WIDTH 15

struct playerType {
	int x;
	int y;
};

Player createPlayer(int x, int y){
	Player aPlayer = malloc(sizeof(struct playerType));
	aPlayer->x = x;
	aPlayer->y = y;
	return aPlayer;
}


int getPlayerPositionX(Player aPlayer) {
	return aPlayer->x;
}

int getPlayerPositionY(Player aPlayer) {
	return aPlayer->y;
}
