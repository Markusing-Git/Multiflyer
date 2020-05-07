#include "LoadMenu.h"


typedef struct menu {
    char menuChoices[NUM_MENU][NAME_LENGTH];
    SDL_Color color;
    SDL_Surface* menuSurface[NUM_MENU];
    SDL_Rect pos[NUM_MENU];
    SDL_Texture* textures[NUM_MENU];
}Menu;

PRIVATE Menu createMenu(SDL_Renderer* renderer, Fonts fonts);

int LoadMenu(SDL_Renderer* renderer, SDL_Window* window, int w, int h, char name[], char ip[], LoadMedia media, Fonts fonts, Game_State current, UDP_Client_Config setup, Game_Route *aGameRoute)
{
    //Initalize for loading image
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    //reset game state
    initGamestate(current);

    //plays background music
    Mix_PlayMusic(media->menuMusic, -1);

    Menu newMenu1;
    newMenu1 = createMenu(renderer, fonts);
    bool running = true;
    int x, y;
    SDL_Color selected = { 77 , 255, 0, 0 };
    SDL_Event event;


    //SDL_Event event;
    while (running)
    {
        // Process events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_MOUSEMOTION) {
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < NUM_MENU; i++)
                {
                    // if focus change text to green
                    if (x >= newMenu1.pos[i].x && x <= newMenu1.pos[i].x + newMenu1.pos[i].w && y > newMenu1.pos[i].y && y <= newMenu1.pos[i].y + newMenu1.pos[i].h)
                    {
                        SDL_DestroyTexture(newMenu1.textures[i]);
                        SDL_Surface* temp = TTF_RenderText_Solid(fonts->magical_45, newMenu1.menuChoices[i], selected);
                        newMenu1.textures[i] = SDL_CreateTextureFromSurface(renderer, temp);
                        SDL_FreeSurface(temp);
                    }
                    else
                    {
                        SDL_DestroyTexture(newMenu1.textures[i]);
                        SDL_Surface* temp = TTF_RenderText_Solid(fonts->magical_45, newMenu1.menuChoices[i], newMenu1.color);
                        newMenu1.textures[i] = SDL_CreateTextureFromSurface(renderer, temp);
                        SDL_FreeSurface(temp);
                    }
                }
            }
            // if click!
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                x = event.button.x;
                y = event.button.y;
                // Quit
                if (x >= newMenu1.pos[3].x && x <= newMenu1.pos[3].x + newMenu1.pos[3].w && y > newMenu1.pos[3].y && y <= newMenu1.pos[3].y + newMenu1.pos[3].h)
                {
                    running = false;
                }
                //Controls
                else if (x >= newMenu1.pos[2].x && x <= newMenu1.pos[2].x + newMenu1.pos[2].w && y > newMenu1.pos[2].y && y <= newMenu1.pos[2].y + newMenu1.pos[2].h)
                {
                    control(renderer, media, aGameRoute);
                }
                //Multiplayer
                else if (x >= newMenu1.pos[1].x && x <= newMenu1.pos[1].x + newMenu1.pos[1].w && y > newMenu1.pos[1].y && y <= newMenu1.pos[1].y + newMenu1.pos[1].h)
                {
                    getHostOrClient(renderer, media, aGameRoute);
                    if (*aGameRoute == hostRoute) {
                        enterName(renderer, media, fonts, name);
                        if (hostLobby(renderer, name, current, setup, fonts, aGameRoute)) {
                            running = false;
                            return 1;
                        }
                    }
                    else if(*aGameRoute == clientRoute) {
                        enterName(renderer, media, fonts, name);
                        enterIp(renderer, media, fonts, ip);
                        if (clientLobby(renderer, name, ip, current, fonts, aGameRoute)) {
                            running = false;
                            return 1;
                        }
                    }
                }
                //Start
                else if (x >= newMenu1.pos[0].x && x <= newMenu1.pos[0].x + newMenu1.pos[0].w && y > newMenu1.pos[0].y && y <= newMenu1.pos[0].y + newMenu1.pos[0].h)
                {
                    *aGameRoute = singlePlayerRoute;
                    current->nrOfPlayers = 1; //needs to be set as one for singleplayer game
                    running = false;
                    return 1;
                }
            }
        }

        //If user clicked play again during gameplay
        if (*aGameRoute == hostRoute) {
            if (hostLobby(renderer, name, current, setup, fonts, aGameRoute)) {
                running = false;
                return 1;
            }
        }
        else if (*aGameRoute == clientRoute) {
            if (clientLobby(renderer, name, ip, current, fonts, aGameRoute)) {
                running = false;
                return 1;
            }
        }

        //if user has quit game
        if (*aGameRoute == quitRoute) {
            running = false;
        }

        // Clear screen
        SDL_RenderClear(renderer);
        SDL_RenderCopyEx(renderer, media->menuBackgroundTexture, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
        //Draw
        for (int i = 0; i < NUM_MENU; i++)
            SDL_RenderCopy(renderer, newMenu1.textures[i], NULL, &newMenu1.pos[i]);
        //show what was drawn
        SDL_RenderPresent(renderer);
    }

    //Release resourse
    for (int i = 0; i < NUM_MENU; i++)
    {
        SDL_DestroyTexture(newMenu1.textures[i]);
    }
    return 0;
}


//******************************************************************************************************************************************************************************************************
//******************************************************************************************************************************************************************************************************
//*********************************************************************************    MENU    *********************************************************************************************************
//******************************************************************************************************************************************************************************************************
//******************************************************************************************************************************************************************************************************

PRIVATE Menu createMenu(SDL_Renderer* renderer, Fonts fonts)
{
    // Init Menu choices 
    Menu newMenu;
    strcpy(newMenu.menuChoices[0], "Start");
    strcpy(newMenu.menuChoices[1], "Multiplayer");
    strcpy(newMenu.menuChoices[2], "Controls");
    strcpy(newMenu.menuChoices[3], "Quit");

    // Set text color to white 
    newMenu.color.r = 0;
    newMenu.color.g = 0;
    newMenu.color.b = 0;
    newMenu.color.a = 0;

    // Render text, create texture and realease resources 
    for (int i = 0; i < NUM_MENU; i++)
        newMenu.menuSurface[i] = TTF_RenderText_Solid(fonts->magical_45, newMenu.menuChoices[i], newMenu.color);
    for (int i = 0; i < NUM_MENU; i++)
        newMenu.textures[i] = SDL_CreateTextureFromSurface(renderer, newMenu.menuSurface[i]);
    for (int i = 0; i < NUM_MENU; i++)
        SDL_FreeSurface(newMenu.menuSurface[i]);

    // Define position for texture 
    newMenu.pos[0].x = 450;
    newMenu.pos[0].y = 220;
    newMenu.pos[1].x = 450;
    newMenu.pos[1].y = 270;
    newMenu.pos[2].x = 450;
    newMenu.pos[2].y = 320;
    newMenu.pos[3].x = 450;
    newMenu.pos[3].y = 370;

    // Get the size of texture (weight & high)
    for (int i = 0; i < NUM_MENU; i++) {
        SDL_QueryTexture(newMenu.textures[i], NULL, NULL, &newMenu.pos[i].w, &newMenu.pos[i].h);
    }

    return newMenu;
}


//******************************************************************************************************************************************************************************************************
//******************************************************************************************************************************************************************************************************
//*********************************************************************************    OPTIONS    *********************************************************************************************************
//******************************************************************************************************************************************************************************************************
//******************************************************************************************************************************************************************************************************

void control(SDL_Renderer* renderer, LoadMedia media, Game_Route *aGameRoute)
{
    bool running = true;
    SDL_Event e;

    //Define positonen for Controls
    SDL_Rect controls_pos;
    controls_pos.x = 0;
    controls_pos.y = 0;
    controls_pos.w = 1000;
    controls_pos.h = 600;

    while (running) {

        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
            {
                *aGameRoute = quitRoute;
                running = false;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
            }
        SDL_RenderClear(renderer);
        SDL_RenderCopyEx(renderer, media->controlsTexture, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(renderer);
    }
}

//******************************************************************************************************************************************************************************************************
//******************************************************************************************************************************************************************************************************
//**************************************************************************************** MULTIPLAYER *************************************************************************************************
//******************************************************************************************************************************************************************************************************
//******************************************************************************************************************************************************************************************************

void getHostOrClient(SDL_Renderer* renderer, LoadMedia media, Game_Route *aGameRoute) {
    SDL_Event e;
    int done = true;
    int x, y;


    //Define position for Multiplayer
    SDL_Rect imageH_pos;
    imageH_pos.x = 100;
    imageH_pos.y = 200;
    imageH_pos.w = 354;
    imageH_pos.h = 185;


    //Define position for Multiplayer
    SDL_Rect imageC_pos;
    imageC_pos.x = 520;
    imageC_pos.y = 200;
    imageC_pos.w = 354;
    imageC_pos.h = 185;


    while (done) {

        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
            {
                *aGameRoute = quitRoute;
                done = false;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                *aGameRoute = menuRoute;
                done = false;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                x = e.button.x;
                y = e.button.y;
                if (x >= imageH_pos.x && x <= imageH_pos.x + imageH_pos.w && y > imageH_pos.y && y <= imageH_pos.y + imageH_pos.h)
                {
                    *aGameRoute = hostRoute;
                    done = false;
                }
                else if (x >= imageC_pos.x && x <= imageC_pos.x + imageC_pos.w && y > imageC_pos.y && y <= imageC_pos.y + imageC_pos.h)
                {
                    *aGameRoute = clientRoute;
                    done = false;
                }
            }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, media->hostButtonTexture, NULL, &imageH_pos);
        SDL_RenderCopy(renderer, media->clientButtonTexture, NULL, &imageC_pos);
        SDL_RenderPresent(renderer);
    }
}


void enterName(SDL_Renderer* renderer, LoadMedia media, Fonts fonts, char name[]) {

    SDL_Event event;
    bool done = false;
    bool renderText = true;
    char nameInit[] = "NAME: ";

    //Textbox
    SDL_Rect txRect_pos;
    txRect_pos.x = 170;
    txRect_pos.y = 150;
    txRect_pos.w = 530;
    txRect_pos.h = 65;

    //Name:
    SDL_Color textColor = { 144, 77, 255, 255 };
    SDL_Color initColor = { 255,255,255, 0 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(fonts->cuvert_24, name, textColor);
    SDL_Texture* nameTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Surface* nameInitSurface = TTF_RenderText_Solid(fonts->cuvert_24, nameInit, initColor);
    SDL_Texture* nameInitTexture = SDL_CreateTextureFromSurface(renderer, nameInitSurface);
    SDL_FreeSurface(nameInitSurface);

    SDL_Rect textRect;
    textRect.x = 275;
    textRect.y = 167;
    SDL_QueryTexture(nameTexture, NULL, NULL, &textRect.w, &textRect.h);

    SDL_Rect tInitRect;
    tInitRect.x = 185;
    tInitRect.y = 167;
    SDL_QueryTexture(nameInitTexture, NULL, NULL, &tInitRect.w, &tInitRect.h);

    while (!done) {

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                done = true;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE) 
                {
                    
                    done = true;
                }
                else if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(name) > 0)
                {
                    char* p = name;
                    p++[strlen(p) - 1] = 0;
                    renderText = true;
                }
                else if (event.key.keysym.sym >= 97 && event.key.keysym.sym <= 123 && strlen(name) < 20) {
                    strcat(name, SDL_GetScancodeName(event.key.keysym.scancode));
                    renderText = true;
                }
            }
        }
        //update texture

        if (renderText) {
            textSurface = TTF_RenderText_Solid(fonts->cuvert_24, name, textColor);
            nameTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_QueryTexture(nameTexture, NULL, NULL, &textRect.w, &textRect.h);

            //render
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, media->textboxTexture, NULL, &txRect_pos);
            SDL_RenderCopy(renderer, nameInitTexture, NULL, &tInitRect);
            SDL_RenderCopy(renderer, nameTexture, NULL, &textRect);
            SDL_RenderPresent(renderer);

            renderText = false;
        }
    }

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(nameInitTexture);
    SDL_DestroyTexture(nameTexture);
}



void enterIp(SDL_Renderer* renderer, LoadMedia media, Fonts fonts, char ip[]) {

    SDL_Event event;
    bool done = false;
    bool renderText = true;
    char ipInit[] = "ip-adress: ";


    //Textbox
    SDL_Rect txRect_pos;
    txRect_pos.x = 170;
    txRect_pos.y = 150;
    txRect_pos.w = 530;
    txRect_pos.h = 65;

    SDL_Color textColor = { 144, 77, 255, 255 };
    SDL_Color initColor = { 255,255,255, 0 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(fonts->cuvert_24, ip, textColor);
    SDL_Texture* ipTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Surface* ipInitSurface = TTF_RenderText_Solid(fonts->cuvert_24, ipInit, initColor);
    SDL_Texture* ipInitTexture = SDL_CreateTextureFromSurface(renderer, ipInitSurface);
    SDL_FreeSurface(ipInitSurface);

    SDL_Rect ipRect;
    ipRect.x = 360;
    ipRect.y = 167;
    SDL_QueryTexture(ipTexture, NULL, NULL, &ipRect.w, &ipRect.h);

    SDL_Rect ipInitRect;
    ipInitRect.x = 185;
    ipInitRect.y = 167;
    SDL_QueryTexture(ipInitTexture, NULL, NULL, &ipInitRect.w, &ipInitRect.h);

    while (!done) {

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                done = true;
            }
            else if (event.type == SDL_KEYDOWN)
            {

                if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE) {
                    done = true;
                }
                else if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(ip) > 0)
                {
                    char* p = ip;
                    p++[strlen(p) - 1] = 0;
                    renderText = true;
                }
                else if (event.key.keysym.sym >= 46 && event.key.keysym.sym <= 58 && strlen(ip) < 20) {
                    strcat(ip, SDL_GetScancodeName(event.key.keysym.scancode));
                    renderText = true;
                }
            }
        }
        //update texture

        if (renderText) {
            textSurface = TTF_RenderText_Solid(fonts->cuvert_24, ip, textColor);
            ipTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_QueryTexture(ipTexture, NULL, NULL, &ipRect.w, &ipRect.h);

            //render
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, media->textboxTexture, NULL, &txRect_pos);
            SDL_RenderCopy(renderer, ipInitTexture, NULL, &ipInitRect);
            SDL_RenderCopy(renderer, ipTexture, NULL, &ipRect);
            SDL_RenderPresent(renderer);

            renderText = false;
        }
    }


    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(ipInitTexture);
    SDL_DestroyTexture(ipTexture);
}

//******************************************************************************************************************************************************************************************************
//******************************************************************************************************************************************************************************************************
//*********************************************************************************    scoreboard    ***************************************************************************************************
//******************************************************************************************************************************************************************************************************
//******************************************************************************************************************************************************************************************************

void openScoreBoard(SDL_Renderer* renderer, LoadMedia media, Fonts fonts, Game_State current, Game_Route *aGameRoute) {

    SDL_Event event;
    bool done = false;
    bool renderText = true;
    int x, y;
    char interActives[][NAME_LENGTH] = { "Play Again", "Return to menu" };
    char gameOver[] = "GAME OVER";
    char playerNames[MAX_PLAYERS][NAME_LENGTH] = {" "};
    char scores[MAX_PLAYERS][NAME_LENGTH] = {" "}; //måste fixa nätverks överföring för detta

    if (*aGameRoute != singlePlayerRoute) {
        for (int i = 0; i < current->nrOfPlayers; i++) {
            strcpy(playerNames[i], current->playerNames[i]);
        }
    }
    else 
    {
        strcpy(playerNames[0], "Player");
    }

    SDL_Rect gameOverRect;
    SDL_Rect nameRects[MAX_PLAYERS]; // för scoreboard
    SDL_Rect interActiveRect[2];

    SDL_Surface* gameOverSurface;
    SDL_Surface* nameSurfaces[MAX_PLAYERS]; //för scoreboard
    SDL_Surface* interActivesSurface[2];

    SDL_Texture* gameOverTexture;
    SDL_Texture* nameTextures[MAX_PLAYERS]; //för scoreboard
    SDL_Texture* interActivesTexture[2];


    //Colors
    SDL_Color scoresColor = { 201, 43, 43, 0 };
    SDL_Color black = { 0,0,0,0 };
    SDL_Color selectedColor = { 77 , 255, 0, 0 };


    gameOverSurface = TTF_RenderText_Solid(fonts->magical_45, gameOver, black);
    //Surfaces för scoreboard
    for(int i= 0; i<MAX_PLAYERS; i++)
        nameSurfaces[i] = TTF_RenderText_Solid(fonts->scoreFont_24, playerNames[i], scoresColor);
    for (int i = 0; i < 2; i++)
        interActivesSurface[i] = TTF_RenderText_Solid(fonts->magical_36, interActives[i], black);


    gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    //textures för scoreboard
    for (int i = 0; i < MAX_PLAYERS; i++)
        nameTextures[i] = SDL_CreateTextureFromSurface(renderer, nameSurfaces[i]);
    for (int i = 0; i < 2; i++)
        interActivesTexture[i] = SDL_CreateTextureFromSurface(renderer, interActivesSurface[i]);


    SDL_FreeSurface(gameOverSurface);
    //free surfaces för scoreboard
    for (int i = 0; i <MAX_PLAYERS; i++)
        SDL_FreeSurface(nameSurfaces[i]);
    for (int i = 0; i < 2; i++)
        SDL_FreeSurface(interActivesSurface[i]);

    //Rects
    SDL_Rect scoreboardPos;
    scoreboardPos.x = 200;
    scoreboardPos.y = 20;
    scoreboardPos.w = 650;
    scoreboardPos.h = 550;




    gameOverRect.x = 435;
    gameOverRect.y = 170;

    nameRects[0].x = 350; 
    nameRects[0].y = 215; 
    nameRects[1].x = 350;
    nameRects[1].y = 255;
    nameRects[2].x = 350;
    nameRects[2].y = 295;
    nameRects[3].x = 350;
    nameRects[3].y = 335;


    interActiveRect[0].x = 360;
    interActiveRect[0].y = 380;
    interActiveRect[1].x = 530;
    interActiveRect[1].y = 380;


    SDL_QueryTexture(gameOverTexture, NULL, NULL, &gameOverRect.w, &gameOverRect.h);
    // Get the size of texture (weight & high) for scoreboard
    for (int i = 0; i < MAX_PLAYERS; i++) 
        SDL_QueryTexture(nameTextures[i], NULL, NULL, &nameRects[i].w, &nameRects[i].h);
    for (int i = 0; i < 2; i++)
        SDL_QueryTexture(interActivesTexture[i], NULL, NULL, &interActiveRect[i].w, &interActiveRect[i].h);

       
  

    while (!done) {

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                *aGameRoute = quitRoute;
                done = true;
            }
            else if (event.type == SDL_MOUSEMOTION) {
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < 2; i++)
                {
                    // if focus change text to green
                    if (x >= interActiveRect[i].x && x <= interActiveRect[i].x + interActiveRect[i].w && y > interActiveRect[i].y && y <= interActiveRect[i].y + interActiveRect[i].h)
                    {
                        SDL_DestroyTexture(interActivesTexture[i]);
                        SDL_Surface* temp = TTF_RenderText_Solid(fonts->magical_45, interActives[i], selectedColor);
                        interActivesTexture[i] = SDL_CreateTextureFromSurface(renderer, temp);
                        SDL_FreeSurface(temp);
                        renderText = true;
                    }
                    else
                    {
                        SDL_DestroyTexture(interActivesTexture[i]);
                        SDL_Surface* temp = TTF_RenderText_Solid(fonts->magical_45, interActives[i], black);
                        interActivesTexture[i] = SDL_CreateTextureFromSurface(renderer, temp);
                        SDL_FreeSurface(temp);
                        renderText = true;
                    }
                }
            }
            // if click!
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                x = event.button.x;
                y = event.button.y;
                //play again or retur to menu
                if (x >= interActiveRect[0].x && x <= interActiveRect[0].x + interActiveRect[0].w && y > interActiveRect[0].y && y <= interActiveRect[0].y + interActiveRect[0].h)
                {
                    done = true;
                }
                else if (x >= interActiveRect[1].x && x <= interActiveRect[1].x + interActiveRect[1].w && y > interActiveRect[1].y && y <= interActiveRect[1].y + interActiveRect[1].h)
                {
                    *aGameRoute = menuRoute;
                    done = true;
                }
            }
        }
        if (renderText) {
            SDL_RenderCopy(renderer, media->scoreBoardTexture, NULL, &scoreboardPos);
            SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
            for(int i = 0 ; i< MAX_PLAYERS; i++)
                SDL_RenderCopy(renderer, nameTextures[i], NULL, &nameRects[i]);
            for (int i = 0; i < 2; i++)
                 SDL_RenderCopy(renderer, interActivesTexture[i], NULL, &interActiveRect[i]);
            SDL_RenderPresent(renderer);
        }
    }
}