#include "player.h"

static int playerHeight = 64;
static int playerWidth = 100;

Player createPlayer(int x, int y) {
	Player aPlayer = malloc(sizeof(struct playerType));
	aPlayer->x = x;
	aPlayer->y = y;
	aPlayer->h = playerHeight;
	aPlayer->w = playerWidth;
	aPlayer->alive = true;
	return aPlayer;
}

void renderPlayer(SDL_Renderer* renderer, SDL_Texture* playerTex, SDL_Texture* splashTex, SDL_Rect* playerPos, Player aPLayer,
	SDL_Rect* playerSprites, SDL_Rect* splashSprites, int playerFrame, int splashFrame) {
	if (aPLayer->alive) {
		SDL_RenderCopyEx(renderer, playerTex, &playerSprites[playerFrame / PLAYER_FRAMES], playerPos, 0, NULL, SDL_FLIP_NONE);
	}
	else {
		SDL_RenderCopyEx(renderer, splashTex, &splashSprites[splashFrame / SPLASH_FRAMES], playerPos, 0, NULL, SDL_FLIP_NONE);
		playerPos->x -= 2;
	}
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

