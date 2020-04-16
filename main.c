#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "game_engine.h"
#include "LoadMenu.h"
#include "Network.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600

int main(void) {

    SDL_Window* window = NULL;
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    bool hostOrClient = true;

    char playerName[NAME_LENGTH] = "No-alias";
    char playerIp[IP_LENGTH] = "127.0.0.1";

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    }
    else
    {
        // Create a SDL window
        window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
        if (!window) {
            printf("error creating window: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        }

        // Create a renderer
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, render_flags);
        if (!renderer) {

            printf("error creating renderer: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        else {

            if (LoadMenu(renderer, window, WINDOW_WIDTH, WINDOW_HEIGHT, &hostOrClient, playerName, playerIp)) {
                //Starts game engine
                if (hostOrClient)
                    startGame(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
                else
                    startClientGame(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
            }
            else {
                SDL_DestroyRenderer(renderer);
            }
        }

    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}