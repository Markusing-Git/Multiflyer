#include "player.h"

static int playerWidth = 64;
static int playerHeight = 100;

Player createPlayer(int x, int y) {
	Player aPlayer = malloc(sizeof(struct playerType));
	aPlayer->playerPos.x = x;
	aPlayer->playerPos.y = y;
	aPlayer->playerPos.h = playerHeight;
	aPlayer->playerPos.w = playerWidth;
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

