#include "game_engine.h"


bool startGame(SDL_Renderer* renderer, int w, int h, char playerName[], char playerIp[], LoadMedia media, Fonts fonts, Game_State current, Network_Config setup, Game_Route *aGameRoute, Store storeStatus) {

    //************************************CREATE ENVOIRMENT**************************************************************************

    int playerFrame = 0; //Den frame som ska visas
    int splashFrame[MAX_PLAYERS] = { 0 };
    int attackFrame[MAX_PLAYERS] = { 0 };
    int immunityFrame = 0;
    int coinFrame = 0;
    Uint32 obstacleDelay = SDL_GetTicks();
    Uint32 gameOverDelay = 0;
    Uint32 PUSpawnTime = SDL_GetTicks() + POWERUP_TIME_DELAY;
    Uint32 powerDuration = 0;
    Uint32 resurectDelay = 0;
    Uint32 resurectImmunDelay = 0;
    bool gameOverDelayFlag = false;
    int nrOfSoundEffects = 0;
    int backgroundOffset = 0;
    int nrOfPushes = 0;


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
    PowerUp powerUpWrapper = initPowerUp();

    bool space = false;
    Uint32 spaceDelay = SDL_GetTicks() - SPACE_DELAY;

    //Starting network   
    set_Game_state(players, current);
    if (current->nrOfPlayers > 1)
    init_Server_network(setup, current);

    //add coins from previous games
    setPlayerCoins(players[current->localPlayerNr - 1], storeStatus->playerCoins);

    //add skin from store
    setPlayerSkin(players[current->localPlayerNr - 1], storeStatus->skinChoice);

    printf("size of gamestate: %d\n", sizeof(struct Game_State_Type));
    printf("size of gamestate send Client: %d\n", sizeof(struct Game_State_Send_Client_Type));
    printf("size of gamestate send Server: %d\n", sizeof(struct Game_State_Send_Server_Type));

    //***************************************************  STARTING GAME ENGINE  *****************************************************
    while (running)
    {
        //POLLING EVENTS

        pollInputEvents(&event, &running, players[0], input, aGameRoute, spaceDelay);

        //*****************  UPPDATING POSITIONS,INPUTS,MULTIPLATER SENDS AND RECEIVES  ***************************************************

        uppdateInputs(players[0], input, current);

        worldCollision(getPlayerPosAdr(players[0]), players[0], w, h);

        //Uppdaterar frames:en, kodblocket skapar en liten delay i bytet mellan frames:en
        playerFrame++;
        if (playerFrame / 3 == PLAYER_FRAMES) {
            playerFrame = 0;
        }

        for (int i = 0; i < current->nrOfPlayers; i++) {
            if (splashFrame[i] != SPLASH_FRAMES * 11 && getPlayerStatus(players[i]) == false) {
                splashFrame[i]++;
            }
            else if (getPlayerStatus(players[i]) == true) {
                splashFrame[i] = 0;
            }
        }

        for (int i = 0; i < current->nrOfPlayers; i++) {
            if(getPlayerAttack(players[i]))
            attackFrame[i]++;
            if (attackFrame[i] / 3 == ATTACK_FRAMES)
                attackFrame[i] = 0;
        }

        //handles obstacles
        if (SDL_GetTicks() >= obstacleDelay + TIME_DELAY) {
            newObstacle(obstacles, w, h);
            SetObstacle(current, obstacles);
            obstacleDelay = SDL_GetTicks();
        }
        obsteclesTick(obstacles);
        obstacleCollision(getPlayerPosAdr(players[0]), players[0], obstacles);

        //handles powerUps
        if (PUSpawnTime <= SDL_GetTicks()) {
            do {
                powerUpWrapper = serverSpawnPowerUp(w, h, powerUpWrapper);
            } while (singelPlayerPowers(aGameRoute, powerUpWrapper));
            SetPowerUp(current, powerUpWrapper);
            PUSpawnTime = SDL_GetTicks() + 1000000;
        }
        if (powerUpConsumed(players, powerUpWrapper, current->nrOfPlayers, &powerDuration) || powerUpExpired(powerUpWrapper))
            PUSpawnTime = (SDL_GetTicks() + POWERUP_TIME_DELAY);
        powerUpTick(powerUpWrapper, w, h);

        //handle player powers
        handlePlayerPowers(players[current->localPlayerNr - 1], &resurectDelay, &resurectImmunDelay, &powerDuration);

        checkIfPassed(getPlayerPosAdr(players[0]), players[0], obstacles);

        playerAttack(current, players, &spaceDelay, &nrOfPushes);


        //Make the background scroll to the left
        scrollBackground(media, &backgroundOffset, w, h);
        

        if (current->nrOfPlayers > 1) {
            sendAndReciveServer(current, setup, playerPos, players);
        }
        else {
            SetGameStatePlayerStatus(current, players);
        }

        limitPingServer(current); 

        //*********************************  RENDERING  ***********************************************************************************
        SDL_RenderClear(renderer);
        SDL_RenderCopyEx(renderer, media->backgroundTex, NULL, &media->scrollingBackground[0], 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(renderer, media->backgroundTex, NULL, &media->scrollingBackground[1], 0, NULL, SDL_FLIP_NONE);
        renderObstacles(obstacles, renderer, media->flyTrapTex);
        renderImmunityBar(renderer, media, players[current->localPlayerNr - 1], &immunityFrame);
        renderPlayerPower(renderer, media, players, current->localPlayerNr - 1, current->nrOfPlayers);
        renderPowerUp(renderer, powerUpWrapper, media, &coinFrame);
        renderPlayers(renderer, players, playerFrame, splashFrame, &nrOfSoundEffects, current->nrOfPlayers, media);
        renderAttack(renderer,media,players,current->nrOfPlayers,attackFrame);
        SDL_RenderCopy(renderer, media->scoreBackgroundTex, NULL, &media->scoreBackgroundRect);
        renderScore(players[current->localPlayerNr-1], media, renderer, fonts);
        SDL_RenderPresent(renderer);

        //if game over
        if (gameOver(players, current->nrOfPlayers, &gameOverDelay, &gameOverDelayFlag)) {
            if (SDL_GetTicks() >= gameOverDelay + 2700) {
                openScoreBoard(renderer, media, fonts, current, aGameRoute, obstacles);
                running = false;
            }
        }
    }

    //add coins from current games
    storeStatus->playerCoins = getPlayerCoins(players[current->localPlayerNr - 1]);

    QuitInput(input);
    freePlayers(players, current->nrOfPlayers);
    if (current->nrOfPlayers > 1) {
        resetServerSDLNet(setup, current);
    }
    return true;
}