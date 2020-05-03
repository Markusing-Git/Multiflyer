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