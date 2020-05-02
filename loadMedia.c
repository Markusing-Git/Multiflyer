#include "loadMedia.h"

LoadMedia loadMedia(SDL_Renderer* renderer, bool* running) {
    LoadMedia media = malloc(sizeof(struct loadMedia));
    media->flySurface = IMG_Load("bilder/startFly.png"); //Laddar in spritesheet
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

    media->flyTex = SDL_CreateTextureFromSurface(renderer, media->flySurface); //skapar en texture fr�n spritesheet
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

    SDL_FreeSurface(media->flySurface);
    SDL_FreeSurface(media->flyTrapSurface);
    SDL_FreeSurface(media->flySplashSurface);
    SDL_FreeSurface(media->backgroundSurface);

    //SPRITES
    //Fly sprite https://opengameart.org/content/green-fly-flying-enemy-game-character artist: bevouliin.com
    media->startFlyBlue[0].x = 0;
    media->startFlyBlue[0].y = 0;
    media->startFlyBlue[0].w = 150;
    media->startFlyBlue[0].h = 135;

    media->startFlyBlue[1].x = 150;
    media->startFlyBlue[1].y = 0;
    media->startFlyBlue[1].w = 150;
    media->startFlyBlue[1].h = 135;

    media->startFlyGreen[0].x = 0;
    media->startFlyGreen[0].y = 135;
    media->startFlyGreen[0].w = 150;
    media->startFlyGreen[0].h = 135;

    media->startFlyGreen[1].x = 150;
    media->startFlyGreen[1].y = 135;
    media->startFlyGreen[1].w = 150;
    media->startFlyGreen[1].h = 135;

    media->startFlyRed[0].x = 0;
    media->startFlyRed[0].y = 270;
    media->startFlyRed[0].w = 150;
    media->startFlyRed[0].h = 135;

    media->startFlyRed[1].x = 150;
    media->startFlyRed[1].y = 270;
    media->startFlyRed[1].w = 150;
    media->startFlyRed[1].h = 135;

    media->startFlyYellow[0].x = 0;
    media->startFlyYellow[0].y = 405;
    media->startFlyYellow[0].w = 150;
    media->startFlyYellow[0].h = 135;

    media->startFlyYellow[1].x = 150;
    media->startFlyYellow[1].y = 405;
    media->startFlyYellow[1].w = 150;
    media->startFlyYellow[1].h = 135;

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

  //*****************************************AUDIO********************************************************
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Could not initialize SDL_mixer. Error: %s", Mix_GetError());
        *running = false;
    }

    //Electricshock noise https://opengameart.org/content/electric-buzz artist: themightyglider
    media->electricShock = Mix_LoadWAV("Audio/buzz.ogg");
    if (media->electricShock == NULL) {
        printf("Could not load sound effect. Error: %s", Mix_GetError());
        *running = false;
    }

    //Flying noise: http://soundbible.com/396-Fly-Buzzing.html Artist: Mike Koenig
	media->flyingNoise = Mix_LoadWAV("Audio/FlyNoise.wav");
	if (media->flyingNoise == NULL) 
	{
		printf("Could not load sound effect. Error: %s \n", Mix_GetError());
	}

    //Background music: https://opengameart.org/content/on-my-way-8-bit-loop Artist: DeltaBreaker
    media->menuMusic = Mix_LoadMUS("Audio/OnMyWay.wav");
    if (media->menuMusic == NULL) {
        printf("Could not load music. Error: %s", Mix_GetError());
    }

    Mix_Volume(-1, 50); //Andrar ljud för alla sound effekts, mellan 0 - 128 (MAX för SDL)
    Mix_VolumeMusic(10); //Andrar volym för musik, mellan 0 - 128 (MAX för SDL)

    return media;
}