#include "game_engine.h"

bool loadClientMedia(SDL_Renderer* renderer);

static SDL_Texture* flyTrapTex = NULL;
static SDL_Texture* flyTex = NULL;
static SDL_Texture* flySplashTex = NULL;
static SDL_Surface* flyTrapSurface = NULL;
static SDL_Surface* flySurface = NULL;
static SDL_Surface* flySplashSurface = NULL;
static SDL_Rect playerSprites[PLAYER_FRAMES];
static SDL_Rect splashSprites[SPLASH_FRAMES];


bool startClientGame(SDL_Renderer* renderer, int w, int h) {

    //************************************CREATE ENVOIRMENT**************************************************************************

    int playerFrame = 0; //Den frame som ska visas
    int splashFrame = 0;
    int playerCount = 0;


    Obstacle obstacles = createObstacle(w, h); //dummy obstacle
    Player players[MAX_PLAYERS];
    newPlayer(players, createPlayer(50, 50), &playerCount);
    newPlayer(players, createPlayer(50, 50), &playerCount);
    UDP_Config setup = malloc(sizeof(struct UDP_Config_Type));
    Game_State current = malloc(sizeof(struct Game_State_Type));
    SDL_Rect* pPlayerPos = getPlayerPosAdr(players[0]);
    SDL_Rect* pOpponentPos = getPlayerPosAdr(players[1]);


    bool running = true;
    SDL_Event event;

    //Laddar in spritesheet och skapar de olika frames:en för spritesheetet
    if (!loadClientMedia(renderer)) {
        running = false; //Spelet stängs ner ifall det skett ett error i loadMedia
    }

    //Starting network
    establishConnection();
    int_network("127.0.0.1", 2000, setup);
    create_Game_state(50, 50, current);
    


    //***************************************************  STARTING GAME ENGINE  *****************************************************
    while (running)
    {
        //polling events
        while (SDL_PollEvent(&event)) //När något händer
        {
            switch (event.type)
            {
            case SDL_QUIT: //Om du trycker på X:et
                running = false;
                break;
            case SDL_KEYDOWN: //Trycker på en knapp
                if (getPlayerStatus(players[0]) == true) {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_UP:
                        movePlayerUp(players[0], 5);
                        break;
                    case SDLK_DOWN:
                        movePlayerDown(players[0], 5);
                        break;
                    case SDLK_LEFT:
                        movePlayerLeft(players[0], 5);
                        break;
                    case SDLK_RIGHT:
                        movePlayerRight(players[0], 5);
                        break;
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    }
                }
                break;
            }
        }

        //*****************  UPPDATING POSITIONS,INPUTS,MULTIPLATER SENDS AND RECEIVES  ***************************************************

        SetPlayerAlive(current, getPlayerStatus(players[0]));

        sendAndRecive(current, setup, pPlayerPos, pOpponentPos);

        setPlayerStatus(players[1], current->opponent_alive);

        worldCollision(getPlayerPosAdr(players[0]), players[0], w, h);

        //Uppdaterar frames:en, kodblocket skapar en liten delay i bytet mellan frames:en
        playerFrame++;
        if (playerFrame / 3 == PLAYER_FRAMES) {
            playerFrame = 0;
        }

        if (splashFrame != SPLASH_FRAMES * 11 && getPlayerStatus(players[0]) == false) {
            splashFrame++;
            if (splashFrame / 13 == SPLASH_FRAMES) {
                splashFrame = 0;
            }
        }

        //handles obstacles
        if (current->obstacle_change_flag) {
            newClientObstacle(ReciveObstacle(current), obstacles);
            printf("new");
        }
        obsteclesTick(obstacles);
        obstacleCollision(getPlayerPosAdr(players[0]), players[0], obstacles);



        //*********************************  RENDERING  ***********************************************************************************
        SDL_RenderClear(renderer);
        renderObstacles(obstacles, renderer, flyTrapTex);
        renderPlayer(renderer, flyTex, flySplashTex, getPlayerPosAdr(players[0]), players[0], playerSprites, splashSprites, playerFrame, splashFrame);
        renderPlayer(renderer, flyTex, flySplashTex, getPlayerPosAdr(players[1]), players[1], playerSprites, splashSprites, playerFrame, splashFrame);
        //SDL_RenderCopyEx(renderer, flyTex, &playerSprites[playerFrame / 3], &opponentPos, 0, NULL, SDL_FLIP_NONE); //Visar spriten
        SDL_RenderPresent(renderer);
    }
    return true;
}

bool loadClientMedia(SDL_Renderer* renderer) {
    bool noError = true;

    flySurface = IMG_Load("bilder/flySpriteSheet.png"); //Laddar in spritesheet
    flyTrapSurface = IMG_Load("bilder/flytrapSpriteSheet.png"); //Laddar in spritesheet
    flySplashSurface = IMG_Load("bilder/bloodsplat.png");

    if (flySurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        noError = false;
        return noError;
    }
    if (flyTrapSurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        noError = false;
        return noError;
    }
    if (flySplashSurface == NULL) {
        printf("Unable to load image. Error: %s", SDL_GetError());  //Kollar efter error vid IMG_Load
        noError = false;
        return noError;
    }

    flyTex = SDL_CreateTextureFromSurface(renderer, flySurface); //skapar en texture från spritesheet
    flyTrapTex = SDL_CreateTextureFromSurface(renderer, flyTrapSurface);
    flySplashTex = SDL_CreateTextureFromSurface(renderer, flySplashSurface);

    if (flyTex == NULL)
    {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        noError = false;
        return noError;
    }
    if (flyTrapTex == NULL) {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        noError = false;
        return noError;
    }
    if (flySplashTex == NULL) {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
        noError = false;
        return noError;
    }

    //SPRITES
    playerSprites[0].x = 0;   //Skapar de olika rect för frames:en i spritesheet, vår spritesheet har 3 frames därav 3 rects
    playerSprites[0].y = 0;
    playerSprites[0].w = 117;
    playerSprites[0].h = 150;

    playerSprites[1].x = 117;
    playerSprites[1].y = 0;
    playerSprites[1].w = 117;
    playerSprites[1].h = 150;

    playerSprites[2].x = 234;
    playerSprites[2].y = 0;
    playerSprites[2].w = 117;
    playerSprites[2].h = 150;

    //splash sprites by https://opengameart.org/users/pwl artist: PWL

    splashSprites[0].x = 960;
    splashSprites[0].y = 0;
    splashSprites[0].w = 480;
    splashSprites[0].h = 480;

    splashSprites[1].x = 1440;
    splashSprites[1].y = 0;
    splashSprites[1].w = 480;
    splashSprites[1].h = 480;

    splashSprites[2].x = 1920;
    splashSprites[2].y = 0;
    splashSprites[2].w = 480;
    splashSprites[2].h = 480;

    splashSprites[3].x = 2400;
    splashSprites[3].y = 0;
    splashSprites[3].w = 480;
    splashSprites[3].h = 480;

    splashSprites[4].x = 2880;
    splashSprites[4].y = 0;
    splashSprites[4].w = 480;
    splashSprites[4].h = 480;

    splashSprites[5].x = 3360;
    splashSprites[5].y = 0;
    splashSprites[5].w = 480;
    splashSprites[5].h = 480;

    splashSprites[6].x = 3840;
    splashSprites[6].y = 0;
    splashSprites[6].w = 480;
    splashSprites[6].h = 480;

    splashSprites[7].x = 4320;
    splashSprites[7].y = 0;
    splashSprites[7].w = 480;
    splashSprites[7].h = 480;

    splashSprites[8].x = 4800;
    splashSprites[8].y = 0;
    splashSprites[8].w = 480;
    splashSprites[8].h = 480;

    splashSprites[9].x = 5280;
    splashSprites[9].y = 0;
    splashSprites[9].w = 480;
    splashSprites[9].h = 480;

    splashSprites[10].x = 5760;
    splashSprites[10].y = 0;
    splashSprites[10].w = 480;
    splashSprites[10].h = 480;

    splashSprites[12].x = 6240;
    splashSprites[12].y = 0;
    splashSprites[12].w = 480;
    splashSprites[12].h = 480;

    splashSprites[13].x = 6720;
    splashSprites[13].y = 0;
    splashSprites[13].w = 480;
    splashSprites[13].h = 480;

    return noError; //Returnerar errorchecken
}