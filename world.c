#include "world.h"

void worldCollision(SDL_Rect* aPlayerPos, Player aPlayer, int screenWidth, int screenHeight) {
	if (aPlayer->alive == true) {
		if (aPlayerPos->x <= 0) {
			aPlayerPos->x = 0;
		}
		if ((aPlayerPos->x + aPlayerPos->w) >= screenWidth) {
			aPlayerPos->x = screenWidth - aPlayerPos->w;
		}
		if (aPlayerPos->y <= 0) {
			aPlayerPos->y = 0;
		}
		if ((aPlayerPos->y + aPlayerPos->h) >= screenHeight) {
			aPlayerPos->y = screenHeight - aPlayerPos->h;
		}
	}
}