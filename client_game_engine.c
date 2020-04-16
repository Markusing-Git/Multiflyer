#include "game_engine.h"


bool startClientGame(SDL_Renderer* renderer, int w, int h, char playerName[], char playerIp[]) {

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

    //Key pushdown
    bool push[4]={0,0,0,0};

    bool running = true;
    SDL_Event event;

    LoadMedia media = loadMedia(renderer, &running);

    //Starting network
    //clientConnection(playerIp, playerName);
    int_network(playerIp, setup);
    create_Game_state(50, 50, current);
    
    //***************************************************  STARTING GAME ENGINE  *****************************************************
    while (running)
    {
        //polling events
        while (SDL_PollEvent(&event)) //N�r n�got h�nder
        {
            switch (event.type)
            {
            case SDL_QUIT: //Om du trycker p� X:et
                running = false;
                break;
            case SDL_KEYDOWN: //Trycker p� en knapp
                if (getPlayerStatus(players[0]) == true) {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_UP:
                        push[0]=1;
                        break;
                    case SDLK_DOWN:
                        push[1]=1;
                        break;
                    case SDLK_LEFT:
                        push[2]=1;
                        break;
                    case SDLK_RIGHT:
                        push[3]=1;
                        break;
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    }
                }
                break;
            case SDL_KEYUP: //Släpper knappen
                if (getPlayerStatus(players[0]) == true) {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_UP:
                        push[0]=0;
                        break;
                    case SDLK_DOWN:
                        push[1]=0;
                        break;
                    case SDLK_LEFT:
                        push[2]=0;
                        break;
                    case SDLK_RIGHT:
                        push[3]=0;
                        break;
                    }
                }
                break;
            }
        }
        //************************* MOVES PLAYER AND SETS PLAYER SPEED *****************************
        if(push[0])
        {
            movePlayerUp(players[0], 5);
        }
        if(push[1])
        {
            movePlayerDown(players[0], 5);
        }
        if(push[2])
        {
            movePlayerLeft(players[0], 5);
        }
        if(push[3])
        {
           movePlayerRight(players[0], 5);
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