#include "LoadMenu.h"


struct menu{
    char menuChoices[NUM_MENU][NAME_LENGTH];
    SDL_Color color;
    TTF_Font* menuFont;
    SDL_Surface* menuSurface[NUM_MENU];
    SDL_Rect pos[NUM_MENU];
    SDL_Texture* textures[NUM_MENU];
    SDL_Surface* menuBackground;
    SDL_Texture* backgroundTexture;
};
typedef struct menu Menu;

Menu createMenu(SDL_Renderer* renderer);

int LoadMenu(SDL_Renderer* renderer, SDL_Window* window, int w, int h, bool* hostOrClient, char name[], char ip[], LoadMedia media, Game_State current, UDP_Client_Config setup)
{
    //Initalize for loading image
    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);   
    
    //************************************AUDIO***************************************************
    Mix_PlayMusic(media->menuMusic, -1);

    Menu newMenu1;
    newMenu1 = createMenu(renderer);
    bool running = true;
    int x,y;
    SDL_Color selected = { 77 , 255, 0, 0 };
    SDL_Event event;

    // Background by 
    // https://www.freeiconspng.com/img/26394
    // https://wallpapertag.com/game-background
    // https://opengameart.org/content/bevouliin-free-flappy-monster-sprite-sheets artis : Bevouliin.com
    // https://opengameart.org/content/blue-bat-sprites artis: bevouliin.com
    // https://opengameart.org/content/green-fly-flying-enemy-game-character artis: bevouliin.com
    // https://opengameart.org/content/happy-fly-enemy-game-character artis: bevouliin.com
    // https://opengameart.org/content/grumpy-bee-enemy-game-character  artis: bevouliin.com

    // Load image
    newMenu1.menuBackground = IMG_Load("bilder/bakgrund.png");
    if(newMenu1.menuBackground == NULL)
    {
        printf("Unable to load image. Error: %s", SDL_GetError());
    }
    newMenu1.backgroundTexture = SDL_CreateTextureFromSurface(renderer, newMenu1.menuBackground);
    if (newMenu1.backgroundTexture == NULL)
    {
        printf("Unable to create texture from surface. Error: %s", SDL_GetError()); //Kollar efter error vid SDL_CreateTextureFromSurface
    }

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
            else if (event.type == SDL_MOUSEMOTION) {
				x = event.motion.x;
				y = event.motion.y;
                for(int i = 0; i < NUM_MENU; i++)
                {   
                    // if focus change text to green
                    if(x>=newMenu1.pos[i].x && x<=newMenu1.pos[i].x+newMenu1.pos[i].w && y>newMenu1.pos[i].y && y<=newMenu1.pos[i].y+newMenu1.pos[i].h)
                    {
                        SDL_DestroyTexture(newMenu1.textures[i]);
                        SDL_Surface* temp = TTF_RenderText_Solid(newMenu1.menuFont, newMenu1.menuChoices[i], selected);
                        newMenu1.textures[i] = SDL_CreateTextureFromSurface(renderer, temp);
                        SDL_FreeSurface(temp);               
                    }
                    else
                    {
                        SDL_DestroyTexture(newMenu1.textures[i]);
                        SDL_Surface* temp = TTF_RenderText_Solid(newMenu1.menuFont, newMenu1.menuChoices[i], newMenu1.color);
                        newMenu1.textures[i] = SDL_CreateTextureFromSurface(renderer, temp);
                        SDL_FreeSurface(temp);
                    }
                }
			}
            // if click!
            else if(event.type==SDL_MOUSEBUTTONDOWN)
            {
                x=event.button.x;
                y=event.button.y;
                // Quit
                if(x>=newMenu1.pos[3].x && x<=newMenu1.pos[3].x+newMenu1.pos[3].w && y>newMenu1.pos[3].y && y<=newMenu1.pos[3].y+newMenu1.pos[3].h)
                {
                    running= false;
                }
                //Controls
                else if(x>=newMenu1.pos[2].x && x<=newMenu1.pos[2].x+newMenu1.pos[2].w && y>newMenu1.pos[2].y && y<=newMenu1.pos[2].y+newMenu1.pos[2].h)
                {
                    control(renderer);
                }
                //Multiplayer
                else if(x>=newMenu1.pos[1].x && x<=newMenu1.pos[1].x+newMenu1.pos[1].w && y>newMenu1.pos[1].y && y<=newMenu1.pos[1].y+newMenu1.pos[1].h)
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
                //Start
                else if(x>=newMenu1.pos[0].x && x<=newMenu1.pos[0].x+newMenu1.pos[0].w && y>newMenu1.pos[0].y && y<=newMenu1.pos[0].y+newMenu1.pos[0].h)
                {                   
                    if (*hostOrClient) {
                        if (hostLobby(renderer, name, current, setup)) {
                            running = false;
                            return 1;
                        }
                    }
                    else {
                        clientLobby(renderer,name,ip,current);
                        running = false;
                        return 1;
                    }
                }
            }
        }
       // Clear screen
        SDL_RenderClear(renderer);
        SDL_RenderCopyEx(renderer, newMenu1.backgroundTexture, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
        //Draw
        for(int i = 0; i < NUM_MENU; i++)
            SDL_RenderCopy(renderer, newMenu1.textures[i], NULL, &newMenu1.pos[i]);
        //show what was drawn
        SDL_RenderPresent(renderer);
    }

    //Release resourse
    for(int i = 0; i < NUM_MENU; i++)
    {
        SDL_DestroyTexture(newMenu1.textures[i]);
    }
    TTF_CloseFont(newMenu1.menuFont);
    TTF_Quit();
    return 0;
}



//**************************************************************************************** MENU *********************************************************************
Menu createMenu(SDL_Renderer* renderer)
{
    // Init Menu choices 
    Menu newMenu;
    strcpy(newMenu.menuChoices[0], "Start");
    strcpy(newMenu.menuChoices[1], "Multiplayer");
    strcpy(newMenu.menuChoices[2], "Option");
    strcpy(newMenu.menuChoices[3], "Quit");

    if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
	}
	newMenu.menuFont = TTF_OpenFont("fonts/Magical.ttf", 45);
	if (newMenu.menuFont == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
	}

    // Set text color to white 
    newMenu.color.r = 0;
    newMenu.color.g = 0;
    newMenu.color.b = 0;
    newMenu.color.a = 0;

    // Render text, create texture and realease resources 
    for (int i = 0; i < NUM_MENU; i++)
		newMenu.menuSurface[i] = TTF_RenderText_Solid(newMenu.menuFont,newMenu.menuChoices[i], newMenu.color);
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



void control(SDL_Renderer* renderer)
{
    bool running = true;
    SDL_Event e;

    SDL_Texture* controls_texture = NULL;
    SDL_Surface* controls = IMG_Load("bilder/instructions1.png");
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
            SDL_RenderCopyEx(renderer, controls_texture, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
            //SDL_RenderCopy(renderer, controls_texture, NULL, &controls_pos);
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