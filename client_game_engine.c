#include "game_engine.h"


bool startClientGame(SDL_Renderer* renderer, int w, int h, char playerName[], char playerIp[], LoadMedia media) {

    //************************************CREATE ENVOIRMENT**************************************************************************

    int playerFrame = 0; //Den frame som ska visas
    int splashFrame = 0;
    int playerCount = 0;
    int nrOfSoundEffects = 0;
    int backgroundOffset = 0;


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
    Inputs input = initInputs();

    //Starting network
    clientConnection(setup,playerIp, playerName,0); //Sätt sync till 1 för att aktivera nätverks sync. Host måste startas innan klient   
    create_Game_state(50, 50, current);

    //***************************************************  STARTING GAME ENGINE  *****************************************************
    while (running)
    {
        //POLLING EVENTS

        pollInputEvents(&event, &running, players[0], input);
        
        //*****************  UPPDATING POSITIONS,INPUTS,MULTIPLATER SENDS AND RECEIVES  ***************************************************

        uppdateInputs(players[0], input);

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
        }
        obsteclesTick(obstacles);
        obstacleCollision(getPlayerPosAdr(players[0]), players[0], obstacles);

        //Make the background scroll to the left
        backgroundOffset -= 1;
        if (backgroundOffset <= -w) {
            backgroundOffset = 0;
        }
        media->scrollingBackground[0].x = backgroundOffset;
        media->scrollingBackground[0].y = 0;
        media->scrollingBackground[0].w = w;
        media->scrollingBackground[0].h = h;

        media->scrollingBackground[1].x = backgroundOffset + w;
        media->scrollingBackground[1].y = 0;
        media->scrollingBackground[1].w = w;
        media->scrollingBackground[1].h = h;

        //*********************************  RENDERING  ***********************************************************************************
        SDL_RenderClear(renderer);
        SDL_RenderCopyEx(renderer, media->backgroundTex, NULL, &media->scrollingBackground[0], 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(renderer, media->backgroundTex, NULL, &media->scrollingBackground[1], 0, NULL, SDL_FLIP_NONE);
        renderObstacles(obstacles, renderer, media->flyTrapTex);
        renderPlayer(renderer, media->flyTex, media->flySplashTex, getPlayerPosAdr(players[0]), players[0], media->startFlyGreen, media->splashSprites, playerFrame, splashFrame, media->electricShock, &nrOfSoundEffects);
        renderPlayer(renderer, media->flyTex, media->flySplashTex, getPlayerPosAdr(players[1]), players[1], media->startFlyRed, media->splashSprites, playerFrame, splashFrame, media->electricShock, &nrOfSoundEffects);
        //SDL_RenderCopyEx(renderer, flyTex, &playerSprites[playerFrame / 3], &opponentPos, 0, NULL, SDL_FLIP_NONE); //Visar spriten
        SDL_RenderPresent(renderer);
    }
    QuitInput(input);
    freePlayers(players, playerCount);
    return true;
}