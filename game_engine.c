#include "game_engine.h"


bool startGame(SDL_Renderer* renderer, int w, int h, char playerName[], char playerIp[], LoadMedia media, Game_State current, UDP_Client_Config setup) {

    //************************************CREATE ENVOIRMENT**************************************************************************

    int playerFrame = 0; //Den frame som ska visas
    int splashFrame = 0;
    int delay = TIME_DELAY;
    int nrOfSoundEffects = 0;
    int backgroundOffset = 0;

    // struct to hold the position and size of the sprite
    initRandomGeneratior();
    Obstacle obstacles = createObstacle(w, h); //innitate start node

    Player players[MAX_PLAYERS];
    SDL_Rect* playerPos[MAX_PLAYERS];
    initPlayers(players, current->nrOfPlayers);

    for (int i = 0; i < current->nrOfPlayers; i++)
        playerPos[i] = getPlayerPosAdr(players[i]);

    bool running = true;
    SDL_Event event;
    Inputs input = initInputs();

    //Starting network

    clientConnection(setup, playerIp, playerName, 0); //Sätt sync till 1 för att aktivera nätverks sync. Host måste startas innan klient   
    create_Game_state(players, current, current->nrOfPlayers);
    int_client_network(playerIp, setup, 2000, 2001);

    //***************************************************  STARTING GAME ENGINE  *****************************************************
    while (running)
    {
        //POLLING EVENTS

        pollInputEvents(&event, &running, players[0], input);

        //*****************  UPPDATING POSITIONS,INPUTS,MULTIPLATER SENDS AND RECEIVES  ***************************************************

        uppdateInputs(players[0], input);

        SetPlayerAlive(current, getPlayerStatus(players[0]), 0);

        if (current->nrOfPlayers > 1)
            sendAndRecive(current, setup, playerPos[0], playerPos[1]);

        setPlayerStatus(players[1], current->player_Alive[1]);

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
        renderPlayers(renderer, players, playerFrame, splashFrame, &nrOfSoundEffects, current->nrOfPlayers, media);
        SDL_RenderPresent(renderer);
    }

    QuitInput(input);
    freePlayers(players, current->nrOfPlayers);
    return true;
}