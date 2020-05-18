#include "loadMedia.h"

// MenuBackground by 
// https://www.freeiconspng.com/img/26394 artist: Ahkâm
// https://wallpapertag.com/game-background artist: Unknown
// https://opengameart.org/content/bevouliin-free-flappy-monster-sprite-sheets artist : Bevouliin.com
// https://opengameart.org/content/blue-bat-sprites artist: bevouliin.com
// https://opengameart.org/content/green-fly-flying-enemy-game-character artist: bevouliin.com
// https://opengameart.org/content/happy-fly-enemy-game-character artist: bevouliin.com
// https://opengameart.org/content/grumpy-bee-enemy-game-character  artist: bevouliin.com

//PowerUps by
//https://opengameart.org/content/pickup-items-icons artist: Cethiel

//Hearts by
//https://opengameart.org/content/health artist: knik1985

LoadMedia loadMedia(SDL_Renderer* renderer, bool* running) {
    LoadMedia media = malloc(sizeof(struct loadMedia));
    media->flySurface = IMG_Load("bilder/startFly.png"); //Laddar in spritesheet
    media->flyTrapSurface = IMG_Load("bilder/electricTrap.png"); //Laddar in spritesheet
    media->flySplashSurface = IMG_Load("bilder/bloodsplat.png");
    media->backgroundSurface = IMG_Load("bilder/background.png");
    media->scoreboardSurface = IMG_Load("bilder/scoreboard.png");
    media->menuBackgroundSurface = IMG_Load("bilder/bakgrund.png");
    media->controlsSurface = IMG_Load("bilder/instructions1.png");
    media->hostButtonSurface = IMG_Load("bilder/Host.png");
    media->clientButtonSurface = IMG_Load("bilder/Client.png");
    media->textboxSurface = IMG_Load("bilder/textbox1.png");
    media->scoreBackgroundSurface = IMG_Load("bilder/scoreBackground.png");
    media->powerUpSurface[0] = IMG_Load("bilder/PUhealth.png");
    media->powerUpSurface[1] = IMG_Load("bilder/PUshield.png");
    media->powerUpSurface[2] = IMG_Load("bilder/PUattack.png");
    media->heartSurface[0] = IMG_Load("bilder/heart1.png");
    media->heartSurface[1] = IMG_Load("bilder/heart2.png");
    media->immunitySurface = IMG_Load("bilder/immunityBar.png");
    media->attackSurface = IMG_Load("bilder/attacksheet.png");
    media->coinSurface = IMG_Load("bilder/coinsprites.png");




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
    if (media->scoreboardSurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        *running = false;
    }
    if (media->menuBackgroundSurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        *running = false;
    }
    if (media->controlsSurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        *running = false;
    }
    if (media->hostButtonSurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        *running = false;
    }
    if (media->clientButtonSurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        *running = false;
    }
    if (media->textboxSurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        *running = false;
    }
    if (media->scoreBackgroundSurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        *running = false;
    }
    for (int i = 0; i < 3; i++) {
        if (media->powerUpSurface[i] == NULL) {
            printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
            *running = false;
        }
    }
    for (int i = 0; i < 2; i++) {
        if (media->heartSurface[i] == NULL) {
            printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
            *running = false;
        }
    }
    if (media->immunitySurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        *running = false;
    }
    if (media->attackSurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        *running = false;
    }

    if (media->coinSurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        *running = false;
    }


    media->flyTex = SDL_CreateTextureFromSurface(renderer, media->flySurface); //skapar en texture fr�n spritesheet
    media->flyTrapTex = SDL_CreateTextureFromSurface(renderer, media->flyTrapSurface);
    media->flySplashTex = SDL_CreateTextureFromSurface(renderer, media->flySplashSurface);
    media->backgroundTex = SDL_CreateTextureFromSurface(renderer, media->backgroundSurface);
    media->scoreBoardTexture = SDL_CreateTextureFromSurface(renderer, media->scoreboardSurface);
    media->menuBackgroundTexture = SDL_CreateTextureFromSurface(renderer, media->menuBackgroundSurface);
    media->controlsTexture = SDL_CreateTextureFromSurface(renderer, media->controlsSurface);
    media->hostButtonTexture = SDL_CreateTextureFromSurface(renderer, media->hostButtonSurface);
    media->clientButtonTexture = SDL_CreateTextureFromSurface(renderer, media->clientButtonSurface);
    media->textboxTexture = SDL_CreateTextureFromSurface(renderer, media->textboxSurface);
    media->scoreBackgroundTex = SDL_CreateTextureFromSurface(renderer, media->scoreBackgroundSurface);
    media->PowerUpTex[0] = SDL_CreateTextureFromSurface(renderer, media->powerUpSurface[0]);
    media->PowerUpTex[1] = SDL_CreateTextureFromSurface(renderer, media->powerUpSurface[1]);
    media->PowerUpTex[2] = SDL_CreateTextureFromSurface(renderer, media->powerUpSurface[2]);
    media->heartTex[0] = SDL_CreateTextureFromSurface(renderer, media->heartSurface[0]);
    media->heartTex[1] = SDL_CreateTextureFromSurface(renderer, media->heartSurface[1]);
    media->immunityTex = SDL_CreateTextureFromSurface(renderer, media->immunitySurface);
    media->attackTex = SDL_CreateTextureFromSurface(renderer,media->attackSurface);
    media->coinTex = SDL_CreateTextureFromSurface(renderer, media->coinSurface);






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
    if (media->scoreBoardTexture == NULL) {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        *running = false;
    }
    if (media->menuBackgroundTexture == NULL) {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        *running = false;
    }
    if (media->controlsTexture == NULL) {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        *running = false;
    }
    if (media->hostButtonTexture == NULL) {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        *running = false;
    }
    if (media->clientButtonTexture == NULL) {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        *running = false;
    }
    if (media->textboxTexture == NULL) {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        *running = false;
    }
    if (media->scoreBackgroundTex == NULL) {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        *running = false;
    }
    for (int i = 0; i < 3; i++) {
        if (media->PowerUpTex[i] == NULL) {
            printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
            *running = false;
        }
    }
    for (int i = 0; i < 2; i++) {
        if (media->heartTex[i] == NULL) {
            printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
            *running = false;
        }
    }
    if (media->immunityTex == NULL) {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        *running = false;
    }
    if (media->attackTex == NULL) {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        *running = false;
    }
    
    if (media->coinTex == NULL) {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        *running = false;
    }

    SDL_FreeSurface(media->flySurface);
    SDL_FreeSurface(media->flyTrapSurface);
    SDL_FreeSurface(media->flySplashSurface);
    SDL_FreeSurface(media->backgroundSurface);
    SDL_FreeSurface(media->scoreboardSurface);
    SDL_FreeSurface(media->menuBackgroundSurface);
    SDL_FreeSurface(media->controlsSurface);
    SDL_FreeSurface(media->hostButtonSurface);
    SDL_FreeSurface(media->clientButtonSurface);
    SDL_FreeSurface(media->textboxSurface);
    SDL_FreeSurface(media->scoreBackgroundSurface);
    for (int i = 0; i < 3; i++)
        SDL_FreeSurface(media->powerUpSurface[i]);
    for (int i = 0; i < 2; i++)
        SDL_FreeSurface(media->heartSurface[i]);
    SDL_FreeSurface(media->immunitySurface);
    SDL_FreeSurface(media->attackSurface);
    SDL_FreeSurface(media->coinSurface);






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

    media->scoreBackgroundRect.x = 700;
    media->scoreBackgroundRect.y = 10;
    media->scoreBackgroundRect.w = 300;
    media->scoreBackgroundRect.h = 90;

    media->scoreRect.x = 740;
    media->scoreRect.y = 25;

    media->heartRect[1].x = media->heartRect[0].x = 5;
    media->heartRect[1].y = media->heartRect[0].y = 25;
    media->heartRect[0].w = 130;
    media->heartRect[0].h = 54;
    media->heartRect[1].w = 280;
    media->heartRect[1].h = 54;

    //Immunity sprites https://opengameart.org/content/health-bar Artist: ab_dias

    media->immunityRect.x = 120;
    media->immunityRect.y = 25;
    media->immunityRect.w = 250;
    media->immunityRect.h = 50;


    media->immunitySprites[0].x = 0;
    media->immunitySprites[0].y = 30;
    media->immunitySprites[0].w = 552;
    media->immunitySprites[0].h = 55;

    media->immunitySprites[1].x = 0;
    media->immunitySprites[1].y = 92;
    media->immunitySprites[1].w = 552;
    media->immunitySprites[1].h = 55;

    media->immunitySprites[2].x = 0;
    media->immunitySprites[2].y = 155;
    media->immunitySprites[2].w = 552;
    media->immunitySprites[2].h = 55;

    media->immunitySprites[3].x = 0;
    media->immunitySprites[3].y = 218;
    media->immunitySprites[3].w = 552;
    media->immunitySprites[3].h = 55;

    media->immunitySprites[4].x = 0;
    media->immunitySprites[4].y = 282;
    media->immunitySprites[4].w = 552;
    media->immunitySprites[4].h = 55;

    media->immunitySprites[5].x = 0;
    media->immunitySprites[5].y = 346;
    media->immunitySprites[5].w = 552;
    media->immunitySprites[5].h = 55;

    media->immunitySprites[6].x = 0;
    media->immunitySprites[6].y = 409;
    media->immunitySprites[6].w = 552;
    media->immunitySprites[6].h = 55;

    media->immunitySprites[7].x = 0;
    media->immunitySprites[7].y = 472;
    media->immunitySprites[7].w = 552;
    media->immunitySprites[7].h = 55;

    media->immunitySprites[8].x = 0;
    media->immunitySprites[8].y = 536;
    media->immunitySprites[8].w = 552;
    media->immunitySprites[8].h = 55;

    media->immunitySprites[9].x = 0;
    media->immunitySprites[9].y = 601;
    media->immunitySprites[9].w = 552;
    media->immunitySprites[9].h = 55;

    media->immunitySprites[10].x = 0;
    media->immunitySprites[10].y = 665;
    media->immunitySprites[10].w = 552;
    media->immunitySprites[10].h = 55;

    media->attackRect[0].x = 0;
    media->attackRect[0].y = 0;
    media->attackRect[0].w = 125;
    media->attackRect[0].h = 122;

    media->attackRect[1].x = 125;
    media->attackRect[1].y = 0;
    media->attackRect[1].w = 125;
    media->attackRect[1].h = 122;

    media->attackRect[2].x = 250;
    media->attackRect[2].y = 0;
    media->attackRect[2].w = 125;
    media->attackRect[2].h = 122;
    //coin sprites Credit dontmind8.blogspot.com Artist:cDontMind8
    media->coinSprites[0].x = 0;
    media->coinSprites[0].y = 0;
    media->coinSprites[0].w = 100;
    media->coinSprites[0].h = 100;

    media->coinSprites[1].x = 100;
    media->coinSprites[1].y = 0;
    media->coinSprites[1].w = 100;
    media->coinSprites[1].h = 100;

    media->coinSprites[2].x = 200;
    media->coinSprites[2].y = 0;
    media->coinSprites[2].w = 100;
    media->coinSprites[2].h = 100;

    media->coinSprites[3].x = 300;
    media->coinSprites[3].y = 0;
    media->coinSprites[3].w = 100;
    media->coinSprites[3].h = 100;

    media->coinSprites[4].x = 400;
    media->coinSprites[4].y = 0;
    media->coinSprites[4].w = 100;
    media->coinSprites[4].h = 100;

    media->coinSprites[5].x = 500;
    media->coinSprites[5].y = 0;
    media->coinSprites[5].w = 100;
    media->coinSprites[5].h = 100;

    


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

//************************************************* TTF FONTS ***************************************************************************************
Fonts loadFonts(void) {

    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }
    Fonts fonts = malloc(sizeof(struct loadFonts));

    fonts->magical_45 = TTF_OpenFont("fonts/Magical.ttf", 45);
    fonts->magical_36 = TTF_OpenFont("fonts/Magical.ttf", 36);
    fonts->cuvert_24 = TTF_OpenFont("fonts/Curvert.otf", 24);
    fonts->cuvert_28 = TTF_OpenFont("fonts/Curvert.otf", 28);
    fonts->cuvert_48 = TTF_OpenFont("fonts/Curvert.otf", 48);
    fonts->cuvert_60 = TTF_OpenFont("fonts/Curvert.otf", 60);
    fonts->ka1_60 = TTF_OpenFont("fonts/ka1.ttf", 60);
    fonts->scoreFont_40 = TTF_OpenFont("fonts/ScoreFont.ttf", 40);
    fonts->scoreFont_24 = TTF_OpenFont("fonts/ScoreFont.ttf", 24);


    if (fonts->magical_45 == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }
    if (fonts->magical_36 == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }
    if (fonts->cuvert_24 == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }
    if (fonts->cuvert_48 == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }
    if (fonts->cuvert_60 == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }
    if (fonts->ka1_60 == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }
    if (fonts->cuvert_28 == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }   
    if (fonts->scoreFont_40 == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }
    if (fonts->scoreFont_24 == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }

    return fonts;
}

void closeFonts(Fonts mediaFonts) {
    TTF_CloseFont(mediaFonts->magical_45);
    TTF_CloseFont(mediaFonts->magical_36);
    TTF_CloseFont(mediaFonts->cuvert_24);
    TTF_CloseFont(mediaFonts->cuvert_28);
    TTF_CloseFont(mediaFonts->cuvert_48);
    TTF_CloseFont(mediaFonts->cuvert_60);
    TTF_CloseFont(mediaFonts->ka1_60);
    TTF_CloseFont(mediaFonts->scoreFont_40);
    TTF_CloseFont(mediaFonts->scoreFont_24);
    free(mediaFonts);
    TTF_Quit();
}

