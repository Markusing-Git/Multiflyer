#include "game_engine.h"


bool startGame(SDL_Renderer* renderer, int w, int h) {

    //************************************CREATE ENVOIRMENT**************************************************************************

    int playerFrame = 0; //Den frame som ska visas
    int splashFrame = 0;
    int delay = TIME_DELAY;
    int playerCount = 0;

    // struct to hold the position and size of the sprite
    initRandomGeneratior();
    Obstacle obstacles = createObstacle(w, h); //innitate start node

    Player players[MAX_PLAYERS];
    newPlayer(players, createPlayer(50, 50), &playerCount);
    newPlayer(players, createPlayer(50, 50), &playerCount);
    UDP_Config setup = malloc(sizeof(struct UDP_Config_Type));
    Game_State current = malloc(sizeof(struct Game_State_Type));
    SDL_Rect* pPlayerPos = getPlayerPosAdr(players[0]);
    SDL_Rect* pOpponentPos = getPlayerPosAdr(players[1]);

    bool running = true;
    SDL_Event event;

    LoadMedia media = loadMedia(renderer, &running);

    //Starting network

    //waitForConnection();
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
        delay--;
        if (delay <= 0) {
            newObstacle(obstacles, w, h);
            SetObstacle(current, obstacles);
            delay = TIME_DELAY;
        }
        obsteclesTick(obstacles);
        obstacleCollision(getPlayerPosAdr(players[0]), players[0], obstacles);


        //*********************************  RENDERING  ***********************************************************************************
        SDL_RenderClear(renderer);
        SDL_RenderCopyEx(renderer, media->backgroundTex, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
        renderObstacles(obstacles, renderer, media->flyTrapTex);
        renderPlayer(renderer, media->flyTex, media->flySplashTex, getPlayerPosAdr(players[0]), players[0], media->playerSprites, media->splashSprites, playerFrame, splashFrame);
        renderPlayer(renderer, media->flyTex, media->flySplashTex, getPlayerPosAdr(players[1]), players[1], media->playerSprites, media->splashSprites, playerFrame, splashFrame);
        //SDL_RenderCopyEx(renderer, flyTex, &playerSprites[playerFrame / 3], &opponentPos, 0, NULL, SDL_FLIP_NONE); //Visar spriten
        SDL_RenderPresent(renderer);
    }
    return true;
}