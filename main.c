#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "game_engine.h"
#include "LoadMenu.h"
#include "Network.h"
#include "loadMedia.h"
#include "constants.h"
//HEYHEJ
int main(void) {

    SDL_Window* window = NULL;
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;;
    bool running = true;

    char playerName[NAME_LENGTH] = "No-alias";
    char playerIp[IP_LENGTH] = "127.0.0.1";

    Game_Route gameRoute = menuRoute;
    Audio settings = malloc(sizeof(struct audio));
    UDP_Client_Config setup = malloc(sizeof(struct UDP_Client_Config_Type));
    Game_State current = malloc(sizeof(struct Game_State_Type));
    initGamestate(current);
    initSettings(settings);


    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    }
    else
    {
        // Create a SDL window
        window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
            //scales all rendering on renderer
            SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

            //loads all the media
            LoadMedia media = loadMedia(renderer, &running);
            Fonts fonts = loadFonts();

            while (running) {
                if (gameRoute == menuRoute || gameRoute == hostRoute || gameRoute == clientRoute) {
                    if (!LoadMenu(renderer, window, WINDOW_WIDTH, WINDOW_HEIGHT, playerName, playerIp, media, fonts, current, setup, &gameRoute, settings)) {
                        running = false; //if user pressed quit
                    }
                    Mix_HaltMusic();
                }
                //Starts game engine
                if (gameRoute == singlePlayerRoute || gameRoute == hostRoute) {
                    startGame(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, playerName, playerIp, media, fonts, current, setup, &gameRoute);
                }
                if(gameRoute == clientRoute){
                        startClientGame(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, playerName, playerIp, media, fonts, current, setup, &gameRoute);
                }
                if (gameRoute == quitRoute) {
                    running = false;
                }
            }

            SDL_DestroyRenderer(renderer);
            closeFonts(fonts);
        }
    }
    //Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}