#include "LoadMenu.h"

int LoadMenu(SDL_Renderer* renderer, SDL_Window* window, int w, int h, bool *hostOrClient)
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
                }
                else if(x>=imageS_pos.x && x<=imageS_pos.x+imageS_pos.w && y>imageS_pos.y && y<=imageS_pos.y+imageS_pos.h)//Start
                {                   
                    running = false;
                    return 1;
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