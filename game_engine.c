#include "game_engine.h"


bool startGame(SDL_Renderer* renderer, int w, int h, char playerName[], char playerIp[], LoadMedia media, Fonts fonts, Game_State current, UDP_Client_Config setup, Game_Route *aGameRoute) {

    //************************************CREATE ENVOIRMENT**************************************************************************

    int playerFrame = 0; //Den frame som ska visas
    int splashFrame[MAX_PLAYERS] = { 0 };
    Uint32 obstacleDelay = SDL_GetTicks();
    Uint32 gameOverDelay = 0;
    bool gameOverDelayFlag = false;
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

    bool space = false;
    Uint32 spaceDelay = SDL_GetTicks();

    //Starting network   
    start_Game_state(players, current);
    if (current->nrOfPlayers > 1)
    init_Server_network(setup, current);

    //***************************************************  STARTING GAME ENGINE  *****************************************************
    while (running)
    {
        //POLLING EVENTS

        pollInputEvents(&event, &running, players[0], input, aGameRoute, &space);

        //*****************  UPPDATING POSITIONS,INPUTS,MULTIPLATER SENDS AND RECEIVES  ***************************************************

        uppdateInputs(players[0], input);

        worldCollision(getPlayerPosAdr(players[0]), players[0], w, h);

        //Uppdaterar frames:en, kodblocket skapar en liten delay i bytet mellan frames:en
        playerFrame++;
        if (playerFrame / 3 == PLAYER_FRAMES) {
            playerFrame = 0;
        }

        for (int i = 0; i < current->nrOfPlayers; i++) {
            if (splashFrame[i] != SPLASH_FRAMES * 11 && getPlayerStatus(players[i]) == false) {
                splashFrame[i]++;
                if (splashFrame[i] / 13 == SPLASH_FRAMES) {
                    splashFrame[i] = 0;
                }
            }
        }

        //handles obstacles
        if (SDL_GetTicks() >= obstacleDelay + TIME_DELAY) {
            newObstacle(obstacles, w, h);
            SetObstacle(current, obstacles);
            obstacleDelay = SDL_GetTicks();
        }
        obsteclesTick(obstacles);
        obstacleCollision(getPlayerPosAdr(players[0]), players[0], obstacles);

        checkIfPassed(getPlayerPosAdr(players[0]), players[0], obstacles);

        if (space == true) {
            if (SDL_GetTicks() >= spaceDelay + SPACE_DELAY) {
                for (int i = 0; i < current->nrOfPlayers; i++) {
                    if (current->localPlayerNr - 1 != i) {
                        current->pushAngle[i] = playerContact(getPlayerPosAdr(players[current->localPlayerNr - 1]), current->player_Pos_X, current->player_Pos_Y, current->nrOfPlayers, current->localPlayerNr);
                        if (current->pushAngle[i] != 0) {
                            current->change_flag = 1;
                            printf("changed");
                        }
                    }
                }
                spaceDelay = SDL_GetTicks();
            }
        }

        if (current->pushAngle[current->localPlayerNr - 1] != 0) {
            pushPlayer(players[current->localPlayerNr - 1], current->pushAngle[current->localPlayerNr - 1]);
            printf("Knuffad");
        }


        space = false;

        //Make the background scroll to the left
        scrollBackground(media, &backgroundOffset, w, h);
        

        if (current->nrOfPlayers > 1)
            sendAndReciveServer(current, setup, playerPos,players);


        //*********************************  RENDERING  ***********************************************************************************
        SDL_RenderClear(renderer);
        SDL_RenderCopyEx(renderer, media->backgroundTex, NULL, &media->scrollingBackground[0], 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(renderer, media->backgroundTex, NULL, &media->scrollingBackground[1], 0, NULL, SDL_FLIP_NONE);
        renderObstacles(obstacles, renderer, media->flyTrapTex);
        renderPlayers(renderer, players, playerFrame, splashFrame, &nrOfSoundEffects, current->nrOfPlayers, media);
        SDL_RenderCopy(renderer, media->scoreBackgroundTex, NULL, &media->scoreBackgroundRect);
        renderScore(players[0], media, renderer, fonts);
        SDL_RenderPresent(renderer);

        if (gameOver(players, current->nrOfPlayers, &gameOverDelay, &gameOverDelayFlag)) {
            if (SDL_GetTicks() >= gameOverDelay + 2700) {
                openScoreBoard(renderer, media, fonts, current, aGameRoute);
                running = false;
            }
        }
    }

    QuitInput(input);
    freePlayers(players, current->nrOfPlayers);
    return true;
}