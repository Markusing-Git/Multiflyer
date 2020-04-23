#include "LoadMenu.h"

int LoadMenu(SDL_Renderer* renderer, SDL_Window* window, int w, int h, bool* hostOrClient, char name[], char ip[])
{
    SDL_Texture* imageS_texture = NULL;
    SDL_Texture* imageM_texture = NULL;
    SDL_Texture* imageC_texture = NULL;
    SDL_Texture* imageQ_texture = NULL;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //Initalize for loading image
    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);   
    // Load an image

    SDL_Surface *imageS = IMG_Load("bilder/Start.png");
    imageS_texture = SDL_CreateTextureFromSurface(renderer,imageS);
    SDL_FreeSurface(imageS); 
    bool running = true;
    //Define position for Start
    SDL_Rect imageS_pos;
    imageS_pos.x = 170;
    imageS_pos.y = 150;
    imageS_pos.w = 150;
    imageS_pos.h = 50;

    SDL_Surface *imageM = IMG_Load("bilder/Multiplayer.png");
    imageM_texture = SDL_CreateTextureFromSurface(renderer,imageM);
    //Define position for Multiplayer
    SDL_Rect imageM_pos;
    imageM_pos.x = 170;
    imageM_pos.y = 225;
    imageM_pos.w = 320;
    imageM_pos.h = 50;
    SDL_FreeSurface(imageM);

    SDL_Surface *imageC = IMG_Load("bilder/controls.png");
    imageC_texture = SDL_CreateTextureFromSurface(renderer,imageC);
    //Define position for Controls
    SDL_Rect imageC_pos;
    imageC_pos.x = 170;
    imageC_pos.y = 300;
    imageC_pos.w = 150;
    imageC_pos.h = 50;
    SDL_FreeSurface(imageC);

    SDL_Surface *imageQ = IMG_Load("bilder/Quit.png");
    imageQ_texture = SDL_CreateTextureFromSurface(renderer,imageQ);
    //Define position for Quit
    SDL_Rect imageQ_pos;
    imageQ_pos.x = 170;
    imageQ_pos.y = 375;
    imageQ_pos.w = 150;
    imageQ_pos.h = 50;
    SDL_FreeSurface(imageQ);
    running = true;
    int x,y;

    SDL_Event event;

    //************************************AUDIO***************************************************
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Could not initialize SDL_mixer. Error: %s", Mix_GetError());
    }

    //Background music: https://opengameart.org/content/on-my-way-8-bit-loop Artist: DeltaBreaker
    Mix_Music* backgroundMusic = Mix_LoadMUS("Audio/OnMyWay.wav");
    if (backgroundMusic == NULL) {
        printf("Could not load music. Error: %s", Mix_GetError());
    }

    Mix_PlayMusic(backgroundMusic, -1);

    //SDL_Event event;
    while(running)
    {
        // Process events
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = false;
            }
            else if(event.type==SDL_MOUSEBUTTONDOWN)// if click!
            {
                x=event.button.x;
                y=event.button.y;
                if(x>=imageQ_pos.x && x<=imageQ_pos.x+imageQ_pos.w && y>imageQ_pos.y && y<=imageQ_pos.y+imageQ_pos.h)// Quit
                {
                    running= false;
                }
                else if(x>=imageC_pos.x && x<=imageC_pos.x+imageC_pos.w && y>imageC_pos.y && y<=imageC_pos.y+imageC_pos.h)//Controls
                {
                    control(renderer);
                }
                else if(x>=imageM_pos.x && x<=imageM_pos.x+imageM_pos.w && y>imageM_pos.y && y<=imageM_pos.y+imageM_pos.h)//Multiplayer
                {
                    getHostOrClient(renderer, hostOrClient);
                    if (*hostOrClient) {
                        enterName(renderer, name);
                    }
                    else {
                        enterName(renderer, name);
                        enterIp(renderer, ip);
                    }
                }
                else if(x>=imageS_pos.x && x<=imageS_pos.x+imageS_pos.w && y>imageS_pos.y && y<=imageS_pos.y+imageS_pos.h)//Start
                {                   
                    if (*hostOrClient) {
                        if (hostLobby(renderer)) {
                            running = false;
                            return 1;
                        }
                    }
                    else {
                        clientLobby(renderer);
                    }
                }
            }
        }
        // Clear screen
        SDL_RenderClear(renderer);
        //Draw
        SDL_RenderCopy(renderer, imageS_texture, NULL, &imageS_pos);
        SDL_RenderCopy(renderer,imageM_texture,NULL,&imageM_pos);
        SDL_RenderCopy(renderer,imageC_texture,NULL,&imageC_pos);
        SDL_RenderCopy(renderer,imageQ_texture,NULL,&imageQ_pos);
        //show what was drawn
        SDL_RenderPresent(renderer);


    }
    //Release resourse
    SDL_DestroyTexture(imageS_texture);
    SDL_DestroyTexture(imageM_texture);
    SDL_DestroyTexture(imageC_texture);
    SDL_DestroyTexture(imageQ_texture);
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    return 0;
}

void control(SDL_Renderer* renderer)
{
    bool running = true;
    SDL_Event e;

    SDL_Texture* controls_texture = NULL;
    SDL_Surface* controls = IMG_Load("bilder/instruction.png");
    controls_texture = SDL_CreateTextureFromSurface(renderer,controls);
    //Define positonen for Controls

    SDL_Rect controls_pos;
    controls_pos.x = 0;
    controls_pos.y = 0;
    controls_pos.w = 1000;
    controls_pos.h = 600;
    SDL_FreeSurface(controls);

    while(running){
        
        while(SDL_PollEvent(&e))
            if(e.type == SDL_QUIT)
            {
                running = false;
            }
            else if(e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
            }
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, controls_texture, NULL, &controls_pos);
            SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(controls_texture);

}



//**************************************************************************************** MULTIPLAYER *********************************************************************
void getHostOrClient(SDL_Renderer* renderer, bool* hostOrClient) {
    SDL_Event e;
    int done = true;
    int x, y;

    SDL_Texture* imageH_texture = NULL;
    SDL_Texture* imageC_texture = NULL;

    SDL_Surface* imageH = IMG_Load("bilder/Host.png");
    imageH_texture = SDL_CreateTextureFromSurface(renderer, imageH);
    //Define position for Multiplayer
    SDL_Rect imageH_pos;
    imageH_pos.x = 100;
    imageH_pos.y = 200;
    imageH_pos.w = 354;
    imageH_pos.h = 185;
    SDL_FreeSurface(imageH);

    SDL_Surface* imageC = IMG_Load("bilder/Client.png");
    imageC_texture = SDL_CreateTextureFromSurface(renderer, imageC);
    //Define position for Multiplayer
    SDL_Rect imageC_pos;
    imageC_pos.x = 520;
    imageC_pos.y = 200;
    imageC_pos.w = 354;
    imageC_pos.h = 185;
    SDL_FreeSurface(imageC);

    while (done) {

        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
            {
                done = false;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                x = e.button.x;
                y = e.button.y;
                if (x >= imageH_pos.x && x <= imageH_pos.x + imageH_pos.w && y > imageH_pos.y && y <= imageH_pos.y + imageH_pos.h)
                {
                    *hostOrClient = true;
                    done = false;
                }
                else if (x >= imageC_pos.x && x <= imageC_pos.x + imageC_pos.w && y > imageC_pos.y && y <= imageC_pos.y + imageC_pos.h)
                {
                    *hostOrClient = false;
                    done = false;
                }
            }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, imageH_texture, NULL, &imageH_pos);
        SDL_RenderCopy(renderer, imageC_texture, NULL, &imageC_pos);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(imageH_texture);
    SDL_DestroyTexture(imageC_texture);
}


void enterName(SDL_Renderer* renderer, char name[]) {

    SDL_Event event;
    bool done = false;
    bool renderText = true;
    char nameInit[] = "NAME: ";

    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }

    TTF_Font* font = TTF_OpenFont("fonts/Curvert.otf", 24);
    if (font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }

    //Textbox
    SDL_Surface* textboxSurface = IMG_Load("bilder/textbox1.png");
    SDL_Texture* textboxTex = SDL_CreateTextureFromSurface(renderer, textboxSurface);
    SDL_Rect txRect_pos;
    txRect_pos.x = 170;
    txRect_pos.y = 150;
    txRect_pos.w = 530;
    txRect_pos.h = 65;
    SDL_FreeSurface(textboxSurface);

    //Name:
    SDL_Color textColor = { 144, 77, 255, 255 };
    SDL_Color initColor = { 255,255,255, 0 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, name, textColor);
    SDL_Texture* nameTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Surface* nameInitSurface = TTF_RenderText_Solid(font, nameInit, initColor);
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

                if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE) {

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
            textSurface = TTF_RenderText_Solid(font, name, textColor);
            nameTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_QueryTexture(nameTexture, NULL, NULL, &textRect.w, &textRect.h);

            //render
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, textboxTex, NULL, &txRect_pos);
            SDL_RenderCopy(renderer, nameInitTexture, NULL, &tInitRect);
            SDL_RenderCopy(renderer, nameTexture, NULL, &textRect);
            SDL_RenderPresent(renderer);

            renderText = false;
        }
    }

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textboxTex);
    SDL_DestroyTexture(nameInitTexture);
    SDL_DestroyTexture(nameTexture);
    TTF_CloseFont(font);
    TTF_Quit();
}



void enterIp(SDL_Renderer* renderer, char ip[]) {

    SDL_Event event;
    bool done = false;
    bool renderText = true;
    char ipInit[] = "ip-adress: ";

    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }

    TTF_Font* font = TTF_OpenFont("fonts/Curvert.otf", 24);
    if (font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }

    //Textbox
    SDL_Surface* textboxSurface = IMG_Load("bilder/textbox1.png");
    SDL_Texture* textboxTex = SDL_CreateTextureFromSurface(renderer, textboxSurface);
    SDL_Rect txRect_pos;
    txRect_pos.x = 170;
    txRect_pos.y = 150;
    txRect_pos.w = 530;
    txRect_pos.h = 65;
    SDL_FreeSurface(textboxSurface);

    SDL_Color textColor = { 144, 77, 255, 255 };
    SDL_Color initColor = { 255,255,255, 0 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, ip, textColor);
    SDL_Texture* ipTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Surface* ipInitSurface = TTF_RenderText_Solid(font, ipInit, initColor);
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
            textSurface = TTF_RenderText_Solid(font, ip, textColor);
            ipTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_QueryTexture(ipTexture, NULL, NULL, &ipRect.w, &ipRect.h);

            //render
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, textboxTex, NULL, &txRect_pos);
            SDL_RenderCopy(renderer, ipInitTexture, NULL, &ipInitRect);
            SDL_RenderCopy(renderer, ipTexture, NULL, &ipRect);
            SDL_RenderPresent(renderer);

            renderText = false;
        }
    }


    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textboxTex);
    SDL_DestroyTexture(ipInitTexture);
    SDL_DestroyTexture(ipTexture);
    TTF_CloseFont(font);
    TTF_Quit();
}