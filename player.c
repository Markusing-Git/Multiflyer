#include "player.h"

PRIVATE int playerWidth = 64;
PRIVATE int playerHeight = 64;

struct playerType {
	SDL_Rect playerPos;
	bool alive;
	bool immunity;
	bool resurected;
	int score;
	int coins;
	PowerType powerType;
	SkinChoices skinChoice;
	bool attack;
};


//PRIVATE FUNCTIONS*****************************************************************************************************************************

//Renders each player
PRIVATE void renderPlayer(SDL_Renderer* renderer, SDL_Texture* playerTex, SDL_Texture* splashTex, SDL_Rect* playerPos, Player aPLayer,
	SDL_Rect* playerSprites, SDL_Rect* splashSprites, int playerFrame, int splashFrame, Mix_Chunk* electricShock, Mix_Chunk* flyingNoise, int* nrOfSoundEffects);

//adds coin to player coins if coin power up is consumed
PRIVATE void addCoinToPlayer(Player aPlayer);

//clearsPowerUps if used (timer for shield usage for attack)
PRIVATE void clearShield(Player aPlayer, Uint32* powerDurationTimer);

//resurects a player if life powerUp active, needs a timer and a timerflag in parameters.
PRIVATE void resurectPlayer(Player aPlayer, Uint32* resurectTimer, Uint32* immunityTimer);
//END OF PRIVATE FUNCTIONS*****************************************************************************************************************************


PUBLIC Player createPlayer(int x, int y) {
	Player aPlayer = malloc(sizeof(struct playerType));
	aPlayer->playerPos.x = x;
	aPlayer->playerPos.y = y;
	aPlayer->playerPos.w = playerWidth;
	aPlayer->playerPos.h = playerHeight;
	aPlayer->alive = true;
	aPlayer->immunity = false;
	aPlayer->resurected = false;
	aPlayer->score = 0;
	aPlayer->coins = 0;
	aPlayer->powerType = none;
	aPlayer->attack = false;
	aPlayer->skinChoice = fly;
	return aPlayer;
}


PUBLIC void renderPlayers(SDL_Renderer* renderer, Player playerList[], int playerFrame, int splashFrame[], int* nrOfSoundEffects, int playerCount, LoadMedia media) {

	switch (playerCount) {
	case 1:
		switch (playerList[0]->skinChoice) {
		case fly:
			renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->startFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case horned:
			renderPlayer(renderer, media->hornedFlyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->hornedFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case goggle:
			renderPlayer(renderer, media->goggleEyeFlyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->goggleEyesFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case angry:
			renderPlayer(renderer, media->angryFlyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->angryFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		}
		break;
	case 2:
		switch (playerList[0]->skinChoice) {
		case fly:
			renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->startFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case horned:
			renderPlayer(renderer, media->hornedFlyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->hornedFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case goggle:
			renderPlayer(renderer, media->goggleEyeFlyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->goggleEyesFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case angry:
			renderPlayer(renderer, media->angryFlyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->angryFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		}
		switch (playerList[1]->skinChoice) {
		case fly:
			renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[1]->playerPos, playerList[1], media->startFlyRed, media->splashSprites, playerFrame, splashFrame[1], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case horned:
			renderPlayer(renderer, media->hornedFlyTex, media->flySplashTex, &playerList[1]->playerPos, playerList[1], media->hornedFlyRed, media->splashSprites, playerFrame, splashFrame[1], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case goggle:
			renderPlayer(renderer, media->goggleEyeFlyTex, media->flySplashTex, &playerList[1]->playerPos, playerList[1], media->goggleEyesFlyRed, media->splashSprites, playerFrame, splashFrame[1], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case angry:
			renderPlayer(renderer, media->angryFlyTex, media->flySplashTex, &playerList[1]->playerPos, playerList[1], media->angryFlyRed, media->splashSprites, playerFrame, splashFrame[1], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		}
		break;
	case 3:
		switch (playerList[0]->skinChoice) {
		case fly:
			renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->startFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case horned:
			renderPlayer(renderer, media->hornedFlyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->hornedFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case goggle:
			renderPlayer(renderer, media->goggleEyeFlyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->goggleEyesFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case angry:
			renderPlayer(renderer, media->angryFlyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->angryFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		}
		switch (playerList[1]->skinChoice) {
		case fly:
			renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[1]->playerPos, playerList[1], media->startFlyRed, media->splashSprites, playerFrame, splashFrame[1], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case horned:
			renderPlayer(renderer, media->hornedFlyTex, media->flySplashTex, &playerList[1]->playerPos, playerList[1], media->hornedFlyRed, media->splashSprites, playerFrame, splashFrame[1], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case goggle:
			renderPlayer(renderer, media->goggleEyeFlyTex, media->flySplashTex, &playerList[1]->playerPos, playerList[1], media->goggleEyesFlyRed, media->splashSprites, playerFrame, splashFrame[1], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case angry:
			renderPlayer(renderer, media->angryFlyTex, media->flySplashTex, &playerList[1]->playerPos, playerList[1], media->angryFlyRed, media->splashSprites, playerFrame, splashFrame[1], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		}
		switch (playerList[2]->skinChoice) {
		case fly:
			renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[2]->playerPos, playerList[2], media->startFlyYellow, media->splashSprites, playerFrame, splashFrame[2], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case horned:
			renderPlayer(renderer, media->hornedFlyTex, media->flySplashTex, &playerList[2]->playerPos, playerList[2], media->hornedFlyYellow, media->splashSprites, playerFrame, splashFrame[2], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case goggle:
			renderPlayer(renderer, media->goggleEyeFlyTex, media->flySplashTex, &playerList[2]->playerPos, playerList[2], media->goggleEyesFlyYellow, media->splashSprites, playerFrame, splashFrame[2], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case angry:
			renderPlayer(renderer, media->angryFlyTex, media->flySplashTex, &playerList[2]->playerPos, playerList[2], media->angryFlyYellow, media->splashSprites, playerFrame, splashFrame[2], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		}
		break;
	case 4:
		switch (playerList[0]->skinChoice) {
		case fly:
			renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->startFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case horned:
			renderPlayer(renderer, media->hornedFlyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->hornedFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case goggle:
			renderPlayer(renderer, media->goggleEyeFlyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->goggleEyesFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case angry:
			renderPlayer(renderer, media->angryFlyTex, media->flySplashTex, &playerList[0]->playerPos, playerList[0], media->angryFlyGreen, media->splashSprites, playerFrame, splashFrame[0], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		}
		switch (playerList[1]->skinChoice) {
		case fly:
			renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[1]->playerPos, playerList[1], media->startFlyRed, media->splashSprites, playerFrame, splashFrame[1], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case horned:
			renderPlayer(renderer, media->hornedFlyTex, media->flySplashTex, &playerList[1]->playerPos, playerList[1], media->hornedFlyRed, media->splashSprites, playerFrame, splashFrame[1], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case goggle:
			renderPlayer(renderer, media->goggleEyeFlyTex, media->flySplashTex, &playerList[1]->playerPos, playerList[1], media->goggleEyesFlyRed, media->splashSprites, playerFrame, splashFrame[1], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case angry:
			renderPlayer(renderer, media->angryFlyTex, media->flySplashTex, &playerList[1]->playerPos, playerList[1], media->angryFlyRed, media->splashSprites, playerFrame, splashFrame[1], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		}
		switch (playerList[2]->skinChoice) {
		case fly:
			renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[2]->playerPos, playerList[2], media->startFlyYellow, media->splashSprites, playerFrame, splashFrame[2], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case horned:
			renderPlayer(renderer, media->hornedFlyTex, media->flySplashTex, &playerList[2]->playerPos, playerList[2], media->hornedFlyYellow, media->splashSprites, playerFrame, splashFrame[2], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case goggle:
			renderPlayer(renderer, media->goggleEyeFlyTex, media->flySplashTex, &playerList[2]->playerPos, playerList[2], media->goggleEyesFlyYellow, media->splashSprites, playerFrame, splashFrame[2], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		case angry:
			renderPlayer(renderer, media->angryFlyTex, media->flySplashTex, &playerList[2]->playerPos, playerList[2], media->angryFlyYellow, media->splashSprites, playerFrame, splashFrame[2], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		}
		switch (playerList[3]->skinChoice) {
		case fly:
			renderPlayer(renderer, media->flyTex, media->flySplashTex, &playerList[3]->playerPos, playerList[3], media->startFlyBlue, media->splashSprites, playerFrame, splashFrame[3], media->electricShock, media->flyingNoise, nrOfSoundEffects);

			break;
		case horned:
			renderPlayer(renderer, media->hornedFlyTex, media->flySplashTex, &playerList[3]->playerPos, playerList[3], media->hornedFlyBlue, media->splashSprites, playerFrame, splashFrame[3], media->electricShock, media->flyingNoise, nrOfSoundEffects);

			break;
		case goggle:
			renderPlayer(renderer, media->goggleEyeFlyTex, media->flySplashTex, &playerList[3]->playerPos, playerList[3], media->goggleEyesFlyBlue, media->splashSprites, playerFrame, splashFrame[3], media->electricShock, media->flyingNoise, nrOfSoundEffects);

			break;
		case angry:
			renderPlayer(renderer, media->angryFlyTex, media->flySplashTex, &playerList[3]->playerPos, playerList[3], media->angryFlyBlue, media->splashSprites, playerFrame, splashFrame[3], media->electricShock, media->flyingNoise, nrOfSoundEffects);
			break;
		}
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

PUBLIC void renderScore(Player aPlayer, LoadMedia media, SDL_Renderer* renderer, Fonts fonts) {
	char scr[50];
	int score = aPlayer->score;

	sprintf(scr, "Score:%d", score);

	SDL_Color colorFront = { 255,255,255 };
	media->score = TTF_RenderText_Solid(fonts->scoreFont_40, scr, colorFront);
	media->scoreTex = SDL_CreateTextureFromSurface(renderer, media->score);
	SDL_QueryTexture(media->scoreTex, NULL, NULL, &media->scoreRect.w, &media->scoreRect.h);
	SDL_RenderCopy(renderer, media->scoreTex, NULL, &media->scoreRect);
	SDL_FreeSurface(media->score);
	SDL_DestroyTexture(media->scoreTex);
}

PUBLIC void renderPlayerPower(SDL_Renderer* renderer, LoadMedia media, Player playerList[], int localPlayer, int playerCount) {

	//renders players health
	if (playerList[localPlayer]->powerType == life && playerList[localPlayer]->resurected == false) {
		SDL_RenderCopy(renderer, media->heartTex[1], NULL, media->heartRect);
	}
	else if(playerList[localPlayer]->alive == true || playerList[localPlayer]->resurected == true){
		SDL_RenderCopy(renderer, media->heartTex[0], NULL, media->heartRect);
	}

	for (int i = 0; i < playerCount; i++) {
		switch (playerList[i]->powerType) {
		case shield:
			if (playerList[i]->alive) {
				media->glowRect.x = playerList[i]->playerPos.x - 29;
				media->glowRect.y = playerList[i]->playerPos.y - 20;
				media->glowRect.w = playerList[i]->playerPos.w + 52;
				media->glowRect.h = playerList[i]->playerPos.h + 55;
				SDL_RenderCopy(renderer, media->PowerUpTex[1], NULL, &media->glowRect);
			}
			break;
		case attack:
			if (playerList[i]->alive) {
				media->glowRect.x = playerList[i]->playerPos.x - 29;
				media->glowRect.y = playerList[i]->playerPos.y - 20;
				media->glowRect.w = playerList[i]->playerPos.w + 52;
				media->glowRect.h = playerList[i]->playerPos.h + 55;
				SDL_RenderCopy(renderer, media->PowerUpTex[2], NULL, &media->glowRect);
			}
			break;
		}
	}
}

PUBLIC void renderImmunityBar(SDL_Renderer* renderer, LoadMedia media, Player aPlayer, int *immunityFrames) {
	if (aPlayer->immunity) {
		(*immunityFrames)++;
		SDL_RenderCopyEx(renderer, media->immunityTex, &media->immunitySprites[(*immunityFrames) / IMMUNITY_FRAMES], &media->immunityRect, 0, NULL, SDL_FLIP_NONE);
	}
	else {
		*immunityFrames = 0;
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

PUBLIC bool getPlayerResurect(Player aPlayer) {
	return aPlayer->resurected;
}

PUBLIC void setPlayerResurect(Player aPlayer, bool resurected) {
	aPlayer->resurected = resurected;
}

PUBLIC int getPlayerPower(Player aPlayer) {
	return aPlayer->powerType;
}

PUBLIC void setPlayerPower(Player aPlayer, PowerType aPowerType) {
	aPlayer->powerType = aPowerType;
}

PUBLIC bool getPlayerImmunity(Player aPlayer) {
	return aPlayer->immunity;
}

PUBLIC int getPlayerScore(Player aPlayer) {
	return aPlayer->score;
}

PUBLIC int getPlayerCoins(Player aPlayer) {
	return aPlayer->coins;
}

PUBLIC void setPlayerCoins(Player aPlayer, int coins) {
	aPlayer->coins = coins;
}

PUBLIC void setPlayerSkin(Player aPlayer, SkinChoices aChoice) {
	aPlayer->skinChoice = aChoice;
}

PUBLIC int getPlayerSkin(Player aPlayer) {
	return aPlayer->skinChoice;
}

PUBLIC bool getPlayerAttack(Player aPlayer){
	return aPlayer->attack;
}
PUBLIC void setPlayerAttack(Player aPlayer, bool attackOrNot)
{
	aPlayer->attack = attackOrNot;
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

PUBLIC void addScore(Player aPlayer) {
	aPlayer->score++;
}


PUBLIC int playerContact(SDL_Rect* playerPos, SDL_Rect* opponentPos) {
	SDL_Rect opponentPlayer;
	SDL_Rect player;

	opponentPlayer.x = opponentPos->x;
	opponentPlayer.y = opponentPos->y;
	opponentPlayer.w = opponentPos->w;
	opponentPlayer.h = opponentPos->h;

	player.x = playerPos->x;
	player.y = playerPos->y;
	player.w = playerPos->w;
	player.h = playerPos->h;

	if (SDL_HasIntersection(&player, &opponentPlayer)) {

		if (player.x < opponentPlayer.x - opponentPlayer.w / 10 && opponentPlayer.y - opponentPlayer.h - 15 < player.y < opponentPlayer.y + 15) {
			return 1;
		}
		else if (player.x > opponentPlayer.x + opponentPlayer.w / 10 && opponentPlayer.y - opponentPlayer.h - 15 < player.y < opponentPlayer.y + 15) {
			return 2;
		}
		else if (player.y < opponentPlayer.y - opponentPlayer.h / 10) {
			return 3;
		}
		else if (player.y > opponentPlayer.y + opponentPlayer.h / 10) {
			return 4;
		}
		else if (opponentPlayer.x - 10 < player.x < opponentPlayer.x + 10 && opponentPlayer.y + 10 < player.y < opponentPlayer.y - 10) {
			return (rand() % 4 - 0) + 1;
		}
	}
	return 0;
}

PRIVATE void resurectPlayer(Player aPlayer, Uint32* resurectTimer, Uint32* immunityTimer) {

	//algorithm to make timing of resurection and obstacle immunity work
	if ((aPlayer->powerType == life && aPlayer->alive == false) || (aPlayer->resurected == true)) { //checks if player has extra life and is dead or if resurection process has begun
		if (aPlayer->resurected == false) {
			(*resurectTimer) = SDL_GetTicks();
			aPlayer->resurected = true;
		}
		else if(SDL_GetTicks() >= ((*resurectTimer) + 3000)) { //resurects player after a timer has been set
			if (!aPlayer->alive) {
				aPlayer->playerPos.x = 50;
				aPlayer->playerPos.y = 100;
				aPlayer->alive = true;
			}
			else if (aPlayer->immunity == false) {
				(*immunityTimer) = SDL_GetTicks();
				aPlayer->immunity = true;
			}
			else if (SDL_GetTicks() >= ((*immunityTimer) + 2000)) { // makes player immune from obstacles until timer is set
				aPlayer->powerType = none;
				aPlayer->resurected = false;
				aPlayer->immunity = false;
			}
		}
	}
}

PRIVATE void clearShield(Player aPlayer, Uint32* powerDurationTimer) {
	if ((SDL_GetTicks() >= ((*powerDurationTimer) + SHIELD_DURATION)) && aPlayer->powerType == shield) {
		aPlayer->powerType = none;
	}
}

PRIVATE void addCoinToPlayer(Player aPlayer) {
	if (aPlayer->powerType == coin) {
		aPlayer->coins++;
		aPlayer->powerType = none;
	}
}

PUBLIC void handlePlayerPowers(Player aPlayer, Uint32* resurectTimer, Uint32* immunityTimer, Uint32* powerDurationTimer) {
	resurectPlayer(aPlayer, resurectTimer, immunityTimer);
	clearShield(aPlayer, powerDurationTimer);
	addCoinToPlayer(aPlayer);
}

PUBLIC bool gameOver(Player playerList[], int playerCount, Uint32* delay, bool* delayFlag) {

	bool allDead = false;

	for (int i = 0; i < playerCount; i++) {
		if (playerList[i]->alive == true || playerList[i]->resurected == true) {
			allDead = false;
			return allDead;
		}
		else
		{
			allDead = true;
		}
	}

	//delay for splash effects to end
	if (*delayFlag == false) {
		(*delay) = SDL_GetTicks();
		(*delayFlag) = true;
	}

	return allDead;
}

PUBLIC void renderAttack(SDL_Renderer *renderer, LoadMedia media, Player playerList[], int playerCount, int attackFrame[])
{
	for (int i = 0; i < playerCount; i++)
		if (playerList[i]->attack)
			SDL_RenderCopyEx(renderer, media->attackTex, &media->attackRect[ attackFrame[i]/ATTACK_FRAMES ], &playerList[i]->playerPos, 0, NULL, SDL_FLIP_NONE);
}