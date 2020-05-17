#include "Store.h"

void initStore(Store status)
{
    //Coloring
    status->white.r = 255;
    status->white.g = 255;
    status->white.b = 255;
    status->white.a = 0;

    status->green.r = 77;
    status->green.g = 255;
    status->green.b = 0;
    status->green.a = 0;

    //Text
    strcpy(status->headLine, "Store");
    strcpy(status->backToMenu, "Back to menu");
    strcpy(status->bank, "Money in bank: ");
    strcpy(status->purchased, "Purchased");
    strcpy(status->price[0], "Cost: 5");
    strcpy(status->price[1], "Cost: 10");
    strcpy(status->price[2], "Cost: 15");
    strcpy(status->price[3], "Cost: 20");
}

void store(SDL_Renderer* renderer, LoadMedia media, Fonts fonts, Store status, int* coins, bool* skinChoice[])
{
    SDL_Event event;
    int x, y, a = 0;
    strcpy(status->coins, "hej"); //Tillfällig

    //Set Bools
    status->done = false;
    status->renderText = true;

    //Skapar surfaces
    status->surfaces[0] = TTF_RenderText_Solid(fonts->cuvert_60, status->headLine, status->white);
    status->surfaces[1] = TTF_RenderText_Solid(fonts->cuvert_28, status->bank, status->white);
    status->surfaces[2] = TTF_RenderText_Solid(fonts->cuvert_28, status->coins, status->white);
    status->surfaces[3] = TTF_RenderText_Solid(fonts->cuvert_28, status->purchased, status->white);
    status->surfaces[4] = TTF_RenderText_Solid(fonts->cuvert_48, status->backToMenu, status->white);
    for (int i = 0; i < OPTIONS; i++)
    {
        status->surfaces[i+5] = TTF_RenderText_Solid(fonts->cuvert_28, status->price[i], status->white);
    }

    //Skapar textures från surfaces
    status->headLine_Tex = SDL_CreateTextureFromSurface(renderer, status->surfaces[0]);
    status->bank_Tex = SDL_CreateTextureFromSurface(renderer, status->surfaces[1]);
    status->coins_Tex = SDL_CreateTextureFromSurface(renderer, status->surfaces[2]);
    status->purchased_Tex = SDL_CreateTextureFromSurface(renderer, status->surfaces[3]);
    status->backToMenu_Tex = SDL_CreateTextureFromSurface(renderer, status->surfaces[4]);
    for (int i = 0; i < OPTIONS; i++)
    {
        status->price_Tex[i] = SDL_CreateTextureFromSurface(renderer, status->surfaces[i+5]);
    }

    //Free surfaces 
	for (int i = 0; i < STORE_SURFACES; i++)
    {
		SDL_FreeSurface(status->surfaces[i]);
    }

    //Positioner for rects
    status->headLine_Rect.x = 370;
    status->headLine_Rect.y = 10;
    SDL_QueryTexture(status->headLine_Tex, NULL, NULL, &status->headLine_Rect.w, &status->headLine_Rect.h);

    status->bank_Rect.x = 650;
    status->bank_Rect.y = 10;
    SDL_QueryTexture(status->bank_Tex, NULL, NULL, &status->bank_Rect.w, &status->bank_Rect.h);

    status->coins_Rect.x = 920;
    status->coins_Rect.y = 10;
    SDL_QueryTexture(status->coins_Tex, NULL, NULL, &status->coins_Rect.w, &status->coins_Rect.h);

    status->purchased_Rect.x = 50;
    status->purchased_Rect.y = 400;
    SDL_QueryTexture(status->purchased_Tex, NULL, NULL, &status->purchased_Rect.w, &status->purchased_Rect.h);

    status->backToMenu_Rect.x = 300;
    status->backToMenu_Rect.y = 500;
    SDL_QueryTexture(status->backToMenu_Tex, NULL, NULL, &status->backToMenu_Rect.w, &status->backToMenu_Rect.h);

	for (int i = 0; i < OPTIONS; i++)
    {
        status->price_Rect[i].x = (100 + a);
        status->price_Rect[i].y = 400;
        SDL_QueryTexture(status->price_Tex[i], NULL, NULL, &status->price_Rect[i].w, &status->price_Rect[i].h);
        a  = (a + 220);
    }

    //Val av status
    while(!status->done)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                status->done = true;
            }
            else if (event.type == SDL_MOUSEMOTION) 
            {
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < OPTIONS; i++)
                {
                    //Om fokus, andra till gron text
                    if (x >= status->price_Rect[i].x && x <= status->price_Rect[i].x + status->price_Rect[i].w && y > status->price_Rect[i].y && y <= status->price_Rect[i].y + status->price_Rect[i].h)
                    {
                        SDL_DestroyTexture(status->price_Tex[i]);
                        SDL_Surface* temp = TTF_RenderText_Solid(fonts->cuvert_28, status->price[i], status->green);
                        status->price_Tex[i] = SDL_CreateTextureFromSurface(renderer, temp);
                        SDL_FreeSurface(temp);
                    }
                    else if (x >= status->backToMenu_Rect.x && x <= status->backToMenu_Rect.x + status->backToMenu_Rect.w && y > status->backToMenu_Rect.y && y <= status->backToMenu_Rect.y + status->backToMenu_Rect.h)
                    {
                        SDL_DestroyTexture(status->backToMenu_Tex);
                        SDL_Surface* temp = TTF_RenderText_Solid(fonts->cuvert_48, status->backToMenu, status->green);
                        status->backToMenu_Tex = SDL_CreateTextureFromSurface(renderer, temp);
                        SDL_FreeSurface(temp);
                    }
                    else
                    {
                        SDL_DestroyTexture(status->price_Tex[i]);
                        SDL_DestroyTexture(status->backToMenu_Tex);
                        SDL_Surface* temp = TTF_RenderText_Solid(fonts->cuvert_28, status->price[i], status->white);
                        SDL_Surface* tmp = TTF_RenderText_Solid(fonts->cuvert_48, status->backToMenu, status->white);
                        status->price_Tex[i] = SDL_CreateTextureFromSurface(renderer, temp);
                        status->backToMenu_Tex = SDL_CreateTextureFromSurface(renderer, tmp);
                        SDL_FreeSurface(temp);
                        SDL_FreeSurface(tmp);
                    }
                }
                status->renderText = true;
			}
            //Om knapptryck 
            else if(event.type==SDL_MOUSEBUTTONDOWN)
            {
                x=event.button.x;
                y=event.button.y;
                for (int i = 0; i < OPTIONS; i++)
                {
                    //Tryck pa nagot skin
                    if(x >= status->price_Rect[i].x && x <= status->price_Rect[i].x + status->price_Rect[i].w && y > status->price_Rect[i].y && y <= status->price_Rect[i].y + status->price_Rect[i].h)
                    {
                        strcpy(status->price[i], status->purchased);
                        status->renderText = true;

                    SDL_DestroyTexture(status->price_Tex[i]);
                    SDL_Surface* temp = TTF_RenderText_Solid(fonts->cuvert_28, status->price[i], status->white);
                    status->price_Tex[i] = SDL_CreateTextureFromSurface(renderer, temp);
                    SDL_QueryTexture(status->price_Tex[i], NULL, NULL, &status->price_Rect[i].w, &status->price_Rect[i].h);
                    SDL_FreeSurface(temp);
                    status->renderText = true;
                    }
                    //Tryck pa back to menu
                    else if(x >= status->backToMenu_Rect.x && x <= status->backToMenu_Rect.x + status->backToMenu_Rect.w && y > status->backToMenu_Rect.y && y <= status->backToMenu_Rect.y + status->backToMenu_Rect.h)
                    {
                        status->renderText = true;
                        status->done = true;
                    }
                }
            }
        }

        //Rendrar
        if(status->renderText)
        {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, status->headLine_Tex, NULL, &status->headLine_Rect);
            SDL_RenderCopy(renderer, status->bank_Tex, NULL, &status->bank_Rect);
            SDL_RenderCopy(renderer, status->coins_Tex, NULL, &status->coins_Rect);
            SDL_RenderCopy(renderer, status->backToMenu_Tex, NULL, &status->backToMenu_Rect);
            for (int i = 0; i < OPTIONS; i++)
            {
                SDL_RenderCopy(renderer, status->price_Tex[i], NULL, &status->price_Rect[i]);
            }
            SDL_RenderPresent(renderer);
            status->renderText = false;
        }
    }

    //Destroy textures
    SDL_DestroyTexture(status->headLine_Tex);
    SDL_DestroyTexture(status->bank_Tex);
    SDL_DestroyTexture(status->coins_Tex);
    SDL_DestroyTexture(status->purchased_Tex);
    SDL_DestroyTexture(status->backToMenu_Tex);
    for (int i = 0; i < OPTIONS; i++)
    {
        SDL_DestroyTexture(status->price_Tex[i]);
    }
}