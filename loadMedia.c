#include "loadMedia.h"

LoadMedia loadMedia(SDL_Renderer* renderer, bool* running) {
    LoadMedia media = malloc(sizeof(struct loadMedia));
    media->flySurface = IMG_Load("bilder/flySpriteSheet.png"); //Laddar in spritesheet
    media->flyTrapSurface = IMG_Load("bilder/electricTrap.png"); //Laddar in spritesheet
    media->flySplashSurface = IMG_Load("bilder/bloodsplat.png");
    media->backgroundSurface = IMG_Load("bilder/background.png");

    if (media->flySurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        *running = false;
    }
    if (media->flyTrapSurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        *running = false;
    }
    if (media->flySplashSurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        *running = false;
    }
    if (media->backgroundSurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        *running = false;
    }

    media->flyTex = SDL_CreateTextureFromSurface(renderer, media->flySurface); //skapar en texture från spritesheet
    media->flyTrapTex = SDL_CreateTextureFromSurface(renderer, media->flyTrapSurface);
    media->flySplashTex = SDL_CreateTextureFromSurface(renderer, media->flySplashSurface);
    media->backgroundTex = SDL_CreateTextureFromSurface(renderer, media->backgroundSurface);

    if (media->flyTex == NULL)
    {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        *running = false;
    }
    if (media->flyTrapTex == NULL) {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        *running = false;
    }
    if (media->flySplashTex == NULL) {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        *running = false;
    }
    if (media->backgroundTex == NULL) {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        *running = false;
    }

    //SPRITES
    media->playerSprites[0].x = 0;   //Skapar de olika rect för frames:en i spritesheet, vår spritesheet har 3 frames därav 3 rects
    media->playerSprites[0].y = 0;
    media->playerSprites[0].w = 117;
    media->playerSprites[0].h = 150;

    media->playerSprites[1].x = 117;
    media->playerSprites[1].y = 0;
    media->playerSprites[1].w = 117;
    media->playerSprites[1].h = 150;

    media->playerSprites[2].x = 234;
    media->playerSprites[2].y = 0;
    media->playerSprites[2].w = 117;
    media->playerSprites[2].h = 150;

    //splash sprites by https://opengameart.org/users/pwl artist: PWL

    media->splashSprites[0].x = 960;
    media->splashSprites[0].y = 0;
    media->splashSprites[0].w = 480;
    media->splashSprites[0].h = 480;

    media->splashSprites[1].x = 1440;
    media->splashSprites[1].y = 0;
    media->splashSprites[1].w = 480;
    media->splashSprites[1].h = 480;

    media->splashSprites[2].x = 1920;
    media->splashSprites[2].y = 0;
    media->splashSprites[2].w = 480;
    media->splashSprites[2].h = 480;

    media->splashSprites[3].x = 2400;
    media->splashSprites[3].y = 0;
    media->splashSprites[3].w = 480;
    media->splashSprites[3].h = 480;

    media->splashSprites[4].x = 2880;
    media->splashSprites[4].y = 0;
    media->splashSprites[4].w = 480;
    media->splashSprites[4].h = 480;

    media->splashSprites[5].x = 3360;
    media->splashSprites[5].y = 0;
    media->splashSprites[5].w = 480;
    media->splashSprites[5].h = 480;

    media->splashSprites[6].x = 3840;
    media->splashSprites[6].y = 0;
    media->splashSprites[6].w = 480;
    media->splashSprites[6].h = 480;

    media->splashSprites[7].x = 4320;
    media->splashSprites[7].y = 0;
    media->splashSprites[7].w = 480;
    media->splashSprites[7].h = 480;

    media->splashSprites[8].x = 4800;
    media->splashSprites[8].y = 0;
    media->splashSprites[8].w = 480;
    media->splashSprites[8].h = 480;

    media->splashSprites[9].x = 5280;
    media->splashSprites[9].y = 0;
    media->splashSprites[9].w = 480;
    media->splashSprites[9].h = 480;

    media->splashSprites[10].x = 5760;
    media->splashSprites[10].y = 0;
    media->splashSprites[10].w = 480;
    media->splashSprites[10].h = 480;

    media->splashSprites[12].x = 6240;
    media->splashSprites[12].y = 0;
    media->splashSprites[12].w = 480;
    media->splashSprites[12].h = 480;

    media->splashSprites[13].x = 6720;
    media->splashSprites[13].y = 0;
    media->splashSprites[13].w = 480;
    media->splashSprites[13].h = 480;

    return media;
}