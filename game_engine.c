#include "game_engine.h"


bool startGame(SDL_Renderer* renderer, int w, int h) {

    //Create Envoirment
    bool running = true;
    SDL_Event event;

    //Starting game engine
    while (running)
    {
        // Polling events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        //updating positions,inputs,multiplayer sends and receives.


        // clear the window and render updates
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        // 60 FPS
        SDL_Delay(1000 / 60); 
    }
    return true;
}