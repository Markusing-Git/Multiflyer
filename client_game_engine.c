#include "game_engine.h"


bool startClientGame(SDL_Renderer* renderer, int w, int h, char playerName[], char playerIp[], LoadMedia media, Fonts fonts, Game_State current, UDP_Client_Config setup, Game_Route *aGameRoute, Store storeStatus) {

    //************************************CREATE ENVOIRMENT**************************************************************************

    int playerFrame = 0; //Den frame som ska visas
    int splashFrame[MAX_PLAYERS] = { 0 };
    int attackFrame[MAX_PLAYERS] = { 0 };
    int nrOfSoundEffects = 0;
    int backgroundOffset = 0;
    int nrOfPushes = 0;
    int immunityFrame = 0;
    int coinFrame = 0;
    Uint32 resurectDelay = 0;
    Uint32 resurectImmunDelay = 0;
    Uint32 gameOverDelay = 0;
    Uint32 powerDuration = 0;
    bool gameOverDelayFlag = false;

    Obstacle obstacles = createObstacle(w, h); //dummy obstacle

    Player players[MAX_PLAYERS];
    SDL_Rect* playerPos[MAX_PLAYERS];
    initPlayers(players, current->nrOfPlayers);

    for (int i = 0; i < current->nrOfPlayers; i++)
        playerPos[i] = getPlayerPosAdr(players[i]);

    bool running = true;
    SDL_Event event;
    Inputs input = initInputs();
    PowerUp powerUpWrapper = initPowerUp();

    Uint32 spaceDelay = SDL_GetTicks() - SPACE_DELAY;

    //Starting network   
    start_Game_state(players, current);
    init_client_network(playerIp, setup, current);
    printf("%d\n", current->localPlayerNr);

    //add coins from previous games
    setPlayerCoins(players[current->localPlayerNr - 1], storeStatus->playerCoins);

    //add skin from store
    setPlayerSkin(players[current->localPlayerNr - 1], storeStatus->skinChoice);

    //***************************************************  STARTING GAME ENGINE  *****************************************************
    while (running)
    {
        //POLLING EVENTS

        pollInputEvents(&event, &running, players[current->localPlayerNr - 1], input,aGameRoute, spaceDelay);
        
        //*****************  UPPDATING POSITIONS,INPUTS,MULTIPLATER SENDS AND RECEIVES  ***************************************************

        uppdateInputs(players[current->localPlayerNr - 1], input, current);


        worldCollision(getPlayerPosAdr(players[current->localPlayerNr - 1]), players[current->localPlayerNr - 1], w, h);

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
        if (current->obstacleChangeFlag) 
            newClientObstacle(ReciveObstacle(current), obstacles);
        obsteclesTick(obstacles);
        obstacleCollision(getPlayerPosAdr(players[current->localPlayerNr - 1]), players[current->localPlayerNr - 1], obstacles);

        //handles powerUps
        if (current->powerUpChangeFlag) {
            powerUpWrapper = ReceivePowerUp(current);
            setPowerUpTimer(powerUpWrapper, SDL_GetTicks());
        }
        powerUpTick(powerUpWrapper, w, h);
        powerUpConsumed(players, powerUpWrapper, current->nrOfPlayers, &powerDuration);
        powerUpExpired(powerUpWrapper);

        //handle player powers
        handlePlayerPowers(players[current->localPlayerNr - 1], &resurectDelay, &resurectImmunDelay, &powerDuration);

        checkIfPassed(getPlayerPosAdr(players[current->localPlayerNr-1]), players[current->localPlayerNr - 1], obstacles);

        playerAttack(current, players, &spaceDelay, &nrOfPushes);


        //Make the background scroll to the left
        scrollBackground(media, &backgroundOffset, w, h);

        //Multiplayer function
        sendAndReciveClient(current, setup, playerPos, players);

        if (renderConnectionsClient(current)) {
            *aGameRoute = menuRoute;
            running = false;
        }

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

        //scoreboard
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
    resetClientSDLNet(setup);
    return true;
}