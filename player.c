#include "player.h"

PRIVATE int playerWidth = 64;
PRIVATE int playerHeight = 64;

struct playerType {
	SDL_Rect playerPos;
	bool alive;
	int score;
};

PRIVATE void renderPlayer(SDL_Renderer* renderer, SDL_Texture* playerTex, SDL_Texture* splashTex, SDL_Rect* playerPos, Player aPLayer,
	SDL_Rect* playerSprites, SDL_Rect* splashSprites, int playerFrame, int splashFrame, Mix_Chunk* electricShock, Mix_Chunk* flyingNoise, int* nrOfSoundEffects);

PUBLIC Player createPlayer(int x, int y) {
	Player aPlayer = malloc(sizeof(struct playerType));
	aPlayer->playerPos.x = x;
	aPlayer->playerPos.y = y;
	aPlayer->playerPos.w = playerWidth;
	aPlayer->playerPos.h = playerHeight;
	aPlayer->alive = true;
	aPlayer->score = 0;
	return aPlayer;
}


PUBLIC void renderPlayers(SDL_Renderer* renderer, Player playerList[], int playerFrame, int splashFrame[], int* nrOfSoundEffects, int playerCount, LoadMedia media) {
	switch (playerCount) {
	case 1:
		renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->startFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
		break;
	case 2:
		renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->startFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
		renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[1]->playerPos, playerList[1], media->startFlyRed, media->splashSprites, playerFrame, splashFrame[1], media->electricShock, media->flyingNoise, nrOfSoundEffects);
		break;
	case 3:
		renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->startFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
		renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[1]->playerPos, playerList[1], media->startFlyRed, media->splashSprites, playerFrame, splashFrame[1], media->electricShock, media->flyingNoise, nrOfSoundEffects);
		renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[2]->playerPos, playerList[2], media->startFlyYellow, media->splashSprites, playerFrame, splashFrame[2], media->electricShock, media->flyingNoise, nrOfSoundEffects);
		break;
	case 4:
		renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->startFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
		renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[1]->playerPos, playerList[1], media->startFlyRed, media->splashSprites, playerFrame, splashFrame[1], media->electricShock, media->flyingNoise, nrOfSoundEffects);
		renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[2]->playerPos, playerList[2], media->startFlyYellow, media->splashSprites, playerFrame, splashFrame[2], media->electricShock, media->flyingNoise, nrOfSoundEffects);
		renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[3]->playerPos, playerList[3], media->startFlyBlue, media->splashSprites, playerFrame, splashFrame[3], media->electricShock, media->flyingNoise, nrOfSoundEffects);
		break;
	default: printf("players is NULL");
	}
}

PRIVATE void renderPlayer(SDL_Renderer* renderer, SDL_Texture* playerTex, SDL_Texture* splashTex, SDL_Rect* playerPos, Player aPLayer,
	SDL_Rect* playerSprites, SDL_Rect* splashSprites, int playerFrame, int splashFrame, Mix_Chunk* electricShock, Mix_Chunk* flyingNoise, int* nrOfSoundEffects) {
	if (aPLayer->alive) {
		SDL_RenderCopyEx(renderer, playerTex, &playerSprites[playerFrame / (PLAYER_FRAMES + 1)], playerPos, 0, NULL, SDL_FLIP_NONE);
		getSoundEffect(1, flyingNoise);
	}
	else {
		SDL_RenderCopyEx(renderer, splashTex, &splashSprites[splashFrame / SPLASH_FRAMES], playerPos, 0, NULL, SDL_FLIP_NONE);
		playerPos->x -= 2;
		if (*nrOfSoundEffects == 0) {
			getSoundEffect(2, electricShock);
			(*nrOfSoundEffects) = 1;
		}
		//Inte säker på vart dessa ska ligga:
		//Mix_FreeChunk(electricShock);
		//Mix_FreeChunk(flyingNoise);
	}
}

void getSoundEffect(int alive, Mix_Chunk* soundEffect)
{
	if(alive==1)
	{
		Mix_PlayChannel(-1, soundEffect, -1); //Plays buzzing noise
	}
	else
	{
		Mix_Pause(-1);
		Mix_PlayChannel(2, soundEffect, 0); //Plays electrified noise
	}
}

PUBLIC SDL_Rect* getPlayerPosAdr(Player aPlayer) {
	return &aPlayer->playerPos;
}

PUBLIC bool getPlayerStatus(Player aPlayer) {
	return aPlayer->alive;
}

PUBLIC void setPlayerStatus(Player aPlayer, bool deadOrAlive) {
	aPlayer->alive = deadOrAlive;
}

PUBLIC int getPlayerPoint(Player aPlayer, char cord) {
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

PUBLIC void setPlayerPoint(Player aPlayer, char cord, int value) {
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

PUBLIC void movePlayerUp(Player aPlayer, int speed) {
	aPlayer->playerPos.y -= speed;
}

PUBLIC void movePlayerDown(Player aPlayer, int speed) {
	aPlayer->playerPos.y += speed;
}

PUBLIC void movePlayerLeft(Player aPlayer, int speed) {
	aPlayer->playerPos.x -= speed;
}

PUBLIC void movePlayerRight(Player aPlayer, int speed) {
	aPlayer->playerPos.x += speed;
}

PUBLIC void initPlayers(Player playerList[], int playerCount) {
	for (int i = 0; i <= playerCount; i++) {
		playerList[i] = createPlayer(50, 50);
	}
}

PUBLIC void freePlayers(Player playerList[], int playerCount) {
	for (int i = 0; i < playerCount; i++) {
		free(playerList[i]);
	}
}

void addScore(Player aPlayer) {
	aPlayer->score++;
}

void renderScore(Player aPlayer, LoadMedia media, SDL_Renderer* renderer, Fonts fonts) {
	char scr[50];
	int score = aPlayer->score;

	sprintf(scr, "Score:%d", score);

	SDL_Color colorFront = { 255,255,255 };
	media->score = TTF_RenderText_Solid(fonts->scoreFont_40, &scr, colorFront);
	media->scoreTex = SDL_CreateTextureFromSurface(renderer, media->score);
	SDL_QueryTexture(media->scoreTex, NULL, NULL, &media->scoreRect.w, &media->scoreRect.h);
	SDL_RenderCopy(renderer, media->scoreTex, NULL, &media->scoreRect);
	SDL_FreeSurface(media->score);
	SDL_DestroyTexture(media->scoreTex);
}