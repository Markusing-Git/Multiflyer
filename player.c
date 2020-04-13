#include "player.h"

static int playerWidth = 64;
static int playerHeight = 100;

struct playerType {
	SDL_Rect playerPos;
	bool alive;
};

Player createPlayer(int x, int y) {
	Player aPlayer = malloc(sizeof(struct playerType));
	aPlayer->playerPos.x = x;
	aPlayer->playerPos.y = y;
	aPlayer->playerPos.w = playerWidth;
	aPlayer->playerPos.h = playerHeight;
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

SDL_Rect* getPlayerPosAdr(Player aPlayer) {
	return &aPlayer->playerPos;
}

bool getPlayerStatus(Player aPlayer) {
	return aPlayer->alive;
}

void setPlayerStatus(Player aPlayer, bool deadOrAlive) {
	aPlayer->alive = deadOrAlive;
}

int getPlayerPoint(Player aPlayer, char cord) {
	switch (cord) {
	case 'x':
		return aPlayer->playerPos.x;
	case 'y':
		return aPlayer->playerPos.y;
	case 'w':
		return aPlayer->playerPos.w;
	case 'h':
		return aPlayer->playerPos.h;
	default:
		printf("Error invalid cordinates");
		return 0;
	}
}

void setPlayerPoint(Player aPlayer, char cord, int value) {
	switch (cord) {
	case 'x':
		aPlayer->playerPos.x = value;
		break;
	case 'y':
		aPlayer->playerPos.y = value;
		break;
	case 'w':
		aPlayer->playerPos.w = value;
		break;
	case 'h':
		aPlayer->playerPos.h = value;
		break;
	default:
		printf("Error invalid cordinates");
	}
}

void movePlayerUp(Player aPlayer, int speed) {
	aPlayer->playerPos.y -= speed;
}

void movePlayerDown(Player aPlayer, int speed) {
	aPlayer->playerPos.y += speed;
}

void movePlayerLeft(Player aPlayer, int speed) {
	aPlayer->playerPos.x -= speed;
}

void movePlayerRight(Player aPlayer, int speed) {
	aPlayer->playerPos.x += speed;
}

void newPlayer(Player playerList[], Player aPlayer, int* playerCount) {
	playerList[(*playerCount)++] = aPlayer;
}