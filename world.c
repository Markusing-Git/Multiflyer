#include "world.h"

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