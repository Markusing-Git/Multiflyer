#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


int main(int argc, char* args[]) {

	bool quit = false;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	SDL_Window* gWindow = SDL_CreateWindow("SDL Example",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
		SCREEN_HEIGHT, SDL_WINDOW_SHOWN);



	SDL_UpdateWindowSurface(gWindow);

	while (!quit) {
		//här lägger vi alla events

	}

	SDL_DestroyWindow(gWindow);

	SDL_Quit();

	return 0;
}
