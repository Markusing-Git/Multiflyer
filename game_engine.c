#include "game_engine.h"
#include "player.h"

#define nrOfFrames 3 //Antal frames i spritesheet


bool loadMedia(SDL_Renderer* renderer);

static SDL_Texture* flyTex = NULL;
static SDL_Surface* flySurface = NULL;
static SDL_Rect spriteClips[nrOfFrames];


bool startGame(SDL_Renderer* renderer, int w, int h) {

    int frame = 0; //Den frame som ska visas

    Player player = createPlayer(50, 50);
    SDL_Rect playerPos = { getPlayerPositionX(player), getPlayerPositionY(player), getPlayerHeight(player), getPlayerWidth(player) };

    //Create Envoirment
    bool running = true;
    SDL_Event event;

    //Laddar in spritesheet och skapar de olika frames:en för spritesheetet
    if (!loadMedia(renderer)) {
        running = false; //Spelet stängs ner ifall det skett ett error i loadMedia
    }

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

        //Uppdaterar frames:en, kodblocket skapar en liten delay i bytet mellan frames:en
        frame++;
        if (frame / 3 == nrOfFrames) {
            frame = 0;
        }

        // clear the window and render updates
        SDL_RenderClear(renderer);
        SDL_RenderCopyEx(renderer, flyTex, &spriteClips[frame / 3], &playerPos, 0, NULL, SDL_FLIP_NONE); //Visar spriten
        SDL_RenderPresent(renderer);

    }
    return true;
}

bool loadMedia(SDL_Renderer* renderer) {
    bool noError = true;
    flySurface = IMG_Load("flySpriteSheet.png"); //Laddar in spritesheet
    if (flySurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        noError = false;
    }
    else {
        flyTex = SDL_CreateTextureFromSurface(renderer, flySurface); //skapar en texture från spritesheet
        if (flyTex == NULL)
        {
            flyTex = SDL_CreateTextureFromSurface(renderer, flySurface); //skapar en texture från spritesheet 
            printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
            noError = false;
        }
        else {
            spriteClips[0].x = 0;   //Skapar de olika rect för frames:en i spritesheet, vår spritesheet har 3 frames därav 3 rects
            spriteClips[0].y = 0;
            spriteClips[0].w = 117;
            spriteClips[0].h = 150;

            spriteClips[1].x = 117;
            spriteClips[1].y = 0;
            spriteClips[1].w = 117;
            spriteClips[1].h = 150;

            spriteClips[2].x = 234;
            spriteClips[2].y = 0;
            spriteClips[2].w = 117;
            spriteClips[2].h = 150;
        }
    }
    return noError; //Returnerar errorchecken
}