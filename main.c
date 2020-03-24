#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_timer.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


int main(int argc, char* args[]) {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* gWindow = SDL_CreateWindow("SDL Example",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
		SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	SDL_Surface* gScreenSurface = SDL_GetWindowSurface(gWindow);
	SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4,
	SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

	SDL_FillRect(gScreenSurface, &fillRect, 0x3AD32F);

	SDL_UpdateWindowSurface(gWindow);

	SDL_Delay(5000);

	SDL_DestroyWindow(gWindow);
	return 0;
}
