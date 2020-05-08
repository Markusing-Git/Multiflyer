#include "world.h"

typedef enum  {
	upRight,
	downRight,
	downLeft,
	upLeft
} Direction;

typedef enum powerType {
	life,
	shield,
	attack
} PowerType;

struct powerUp_type {
	SDL_Rect powerPos;
	Direction direction;
	PowerType powerType;
};

//PRIVATE FUNCTION DECLARATIONS
PRIVATE void powerUpWorldCollision(PowerUp aPowerUp, int screenHeight, int screenWidth);


//******************************************************************************************************************************************************************************************************
//******************************************************************************************************************************************************************************************************
//*********************************************************************************    WORLD    ********************************************************************************************************
//******************************************************************************************************************************************************************************************************
//******************************************************************************************************************************************************************************************************

void worldCollision(SDL_Rect* aPlayerPos, Player aPlayer, int screenWidth, int screenHeight) {
	int pixelX = 2;
	int pixelY = 13;
	int pixelW = 4;
	int pixelH = 2;
	if (getPlayerStatus(aPlayer) == true) {
		if (aPlayerPos->x + pixelX <= 0) {
			aPlayerPos->x = 0 - pixelX;
		}
		if ((aPlayerPos->x + aPlayerPos->w - pixelW) >= screenWidth) {
			aPlayerPos->x = screenWidth - aPlayerPos->w + pixelW;
		}
		if (aPlayerPos->y + pixelY <= 0) {
			aPlayerPos->y = 0 - pixelY;
		}
		if ((aPlayerPos->y + aPlayerPos->h - pixelH) >= screenHeight) {
			aPlayerPos->y = screenHeight - aPlayerPos->h + pixelH;
		}
	}
}

void scrollBackground(LoadMedia aMedia, int *aOffset, int w, int h) {
	(*aOffset) -= 1;
	if ((*aOffset) <= -w) {
		(*aOffset) = 0;
	}
	aMedia->scrollingBackground[0].x = (*aOffset);
	aMedia->scrollingBackground[0].y = 0;
	aMedia->scrollingBackground[0].w = w;
	aMedia->scrollingBackground[0].h = h;

	aMedia->scrollingBackground[1].x = (*aOffset) + w;
	aMedia->scrollingBackground[1].y = 0;
	aMedia->scrollingBackground[1].w = w;
	aMedia->scrollingBackground[1].h = h;
}

//******************************************************************************************************************************************************************************************************
//******************************************************************************************************************************************************************************************************
//*********************************************************************************    powerUps    *****************************************************************************************************
//******************************************************************************************************************************************************************************************************
//******************************************************************************************************************************************************************************************************

PUBLIC void spawnPowerUp(PowerUp aPowerUp, int screenWidth, int screenHeight) {
	PowerUp powerUp = malloc(sizeof(struct powerUp_type));
	int rndX = (rand() % screenWidth - 0) + 1;
	int rndY = (rand() % screenHeight - 0) + 1;
	int rndType = (rand() % 3 - 0);
	int rndDir = (rand() % 4 - 0);

	powerUp->powerPos.x = rndX;
	powerUp->powerPos.y = rndY;
	//powerUp->powerPos.w 
	//powerUp->powerPos.h 
	powerUp->powerType = rndType;
	powerUp->direction = rndDir;

	aPowerUp = powerUp;
}

PRIVATE void powerUpWorldCollision(PowerUp aPowerUp, int screenHeight, int screenWidth) {
	int pixelX = 0;
	int pixelY = 0;
	int pixelW = 0;
	int pixelH = 0;

	//changes direction if wall is hit
	if (aPowerUp->powerPos.x + pixelX <= 0) {
		switch (aPowerUp->direction) {
		case downLeft:
			aPowerUp->direction = downRight;
		case upLeft:
			aPowerUp->direction = upRight;
		}
	}
	else if ((aPowerUp->powerPos.x + aPowerUp->powerPos.w - pixelW) >= screenWidth)
	{
		switch (aPowerUp->direction) {
		case downRight:
			aPowerUp->direction = downLeft;
		case upRight:
			aPowerUp->direction = upLeft;
		}
	}
	else if (aPowerUp->powerPos.y + pixelY <= 0)
	{
		switch (aPowerUp->direction) {
		case upRight:
			aPowerUp->direction = downRight;
		case upLeft:
			aPowerUp->direction = downLeft;
		}
	}
	else if ((aPowerUp->powerPos.y + aPowerUp->powerPos.h - pixelH) >= screenHeight)
	{
		switch (aPowerUp->direction) {
		case downLeft:
			aPowerUp->direction = upLeft;
		case downRight:
			aPowerUp->direction = upRight;
		}
	}
}


PUBLIC void powerUpTick(PowerUp aPowerUp, int screenHeight, int screenWidth) {

	powerUpWorldCollision(aPowerUp, screenHeight, screenWidth);

	switch (aPowerUp->direction) {
	case upRight:
		aPowerUp->powerPos.y -= POWERUP_SPEED;
		aPowerUp->powerPos.x += POWERUP_SPEED;
	case downRight:
		aPowerUp->powerPos.y += POWERUP_SPEED;
		aPowerUp->powerPos.x += POWERUP_SPEED;
	case upLeft:
		aPowerUp->powerPos.y -= POWERUP_SPEED;
		aPowerUp->powerPos.x -= POWERUP_SPEED;
	case downLeft:
		aPowerUp->powerPos.y += POWERUP_SPEED;
		aPowerUp->powerPos.x -= POWERUP_SPEED;
	}
}

void renderPowerUp(PowerUp aPowerUp) {

}

/*
render powerup(power up)
*/

/*
powerUpConsumed(Player playerList[], power up)

give player power

destroy power
*/