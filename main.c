#include <stdio.h>
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
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		running = false;
    }
    else
    {
        // Create a SDL window
        window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
        bool running = true;
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