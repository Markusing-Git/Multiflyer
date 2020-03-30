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

void drawPlayer(SDL_Surface* screen, int x, int y) {
	/* Blits the player to the screen */
	SDL_Rect playerRect = { x, y, 25, 25 };
	Uint8 playerColor = SDL_MapRGB(screen->format, 255, 255, 255);
	SDL_FillRect(screen, &playerRect, playerColor);
}