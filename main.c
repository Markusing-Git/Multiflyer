#include <stdio.h>
<<<<<<< HEAD
#include <stdbool.h>
#include <SDL2/SDL.h>
#define width 640
#define height 480

SDL_Window* window = NULL;
int main(void)
{
    // Initialize SDL
    bool running = true;
    if (SDL_Init(SDL_INIT_VIDEO != 0)) {
=======
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>

bool init();
void close();

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;

int main(int argc, char* args[]) {
	
	bool running = false;
	
	if (!init()) {
		printf("Failed to initialize!\n");
		printf("Hej");
		//Hoang
		//jacob
		//Test
		//Joel
	}


	while (!running) {
		//H�r l�gger vi allt som anropas n�r spelet k�rs
		SDL_Delay(10000);
		running = true;
		
		
	}
	
	SDL_UpdateWindowSurface(gWindow);
	close();

	return 0;
}

bool init(void) {
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO != 0)) {
>>>>>>> ec2a5d57218cc061111222bbc151e61a75585df3
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		running = false;
    }
    else
    {
        // Create a SDL window
        window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
        SDL_Event event;
        while(running)
        {
            // Process events
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT)
                {
                    running = false;
                }
            }
        
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}