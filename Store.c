#include "Store.h"

PRIVATE int handlePurchase(Store storeStatus, int iteration);

void initStore(Store storeStatus)
{
    storeStatus->playerCoins = 50;
    storeStatus->selectedRect = 0;
    storeStatus->skinChoice = fly;

    //Coloring
    storeStatus->white.r = 255;
    storeStatus->white.g = 255;
    storeStatus->white.b = 255;
    storeStatus->white.a = 0;

    storeStatus->green.r = 77;
    storeStatus->green.g = 255;
    storeStatus->green.b = 0;
    storeStatus->green.a = 0;

    storeStatus->purchasedSkin[0] = true;
    for (int i = 1; i < OPTIONS; i++)
    {
        storeStatus->purchasedSkin[i] = false;
    }

    int offset = 0;
    for (int i = 0; i < OPTIONS; i++)
    {
        storeStatus->skins[i].x = 85 + offset;
        storeStatus->skins[i].y = 200;
        storeStatus->skins[i].w = 150;
        storeStatus->skins[i].h = 138;
        offset += 230;
    }

    offset = 0;
    for (int i = 0; i < OPTIONS; i++)
    {
        storeStatus->skinBackgroundRect[i].x = 65 + offset;
        storeStatus->skinBackgroundRect[i].y = 200;
        storeStatus->skinBackgroundRect[i].w = 200;
        storeStatus->skinBackgroundRect[i].h = 150;
        offset += 225;
    }

    //Text
    strcpy(storeStatus->headLine, "Store");
    strcpy(storeStatus->backToMenu, "Back to menu");
    strcpy(storeStatus->bank, "Coins:");
    strcpy(storeStatus->purchased, "Purchased");
    strcpy(storeStatus->price[0], "Purchased");
    strcpy(storeStatus->price[1], "Cost: 10");
    strcpy(storeStatus->price[2], "Cost: 15");
    strcpy(storeStatus->price[3], "Cost: 20");
}

void store(SDL_Renderer* renderer, LoadMedia media, Fonts fonts, Store storeStatus)
{
    SDL_Event event;
    int x, y, a = 0;
    sprintf(storeStatus->coins, "%d", storeStatus->playerCoins);

    //Set Bools
    storeStatus->done = false;
    storeStatus->renderText = true;

    //Skapar surfaces
    storeStatus->surfaces[0] = TTF_RenderText_Solid(fonts->cuvert_60, storeStatus->headLine, storeStatus->white);
    storeStatus->surfaces[1] = TTF_RenderText_Solid(fonts->cuvert_28, storeStatus->bank, storeStatus->white);
    storeStatus->surfaces[2] = TTF_RenderText_Solid(fonts->cuvert_28, storeStatus->coins, storeStatus->white);
    storeStatus->surfaces[3] = TTF_RenderText_Solid(fonts->cuvert_28, storeStatus->purchased, storeStatus->white);
    storeStatus->surfaces[4] = TTF_RenderText_Solid(fonts->cuvert_48, storeStatus->backToMenu, storeStatus->white);
    for (int i = 0; i < OPTIONS; i++)
    {
        storeStatus->surfaces[i+5] = TTF_RenderText_Solid(fonts->cuvert_28, storeStatus->price[i], storeStatus->white);
    }

    //Skapar textures från surfaces
    storeStatus->headLine_Tex = SDL_CreateTextureFromSurface(renderer, storeStatus->surfaces[0]);
    storeStatus->bank_Tex = SDL_CreateTextureFromSurface(renderer, storeStatus->surfaces[1]);
    storeStatus->coins_Tex = SDL_CreateTextureFromSurface(renderer, storeStatus->surfaces[2]);
    storeStatus->purchased_Tex = SDL_CreateTextureFromSurface(renderer, storeStatus->surfaces[3]);
    storeStatus->backToMenu_Tex = SDL_CreateTextureFromSurface(renderer, storeStatus->surfaces[4]);
    for (int i = 0; i < OPTIONS; i++)
    {
        storeStatus->price_Tex[i] = SDL_CreateTextureFromSurface(renderer, storeStatus->surfaces[i+5]);
    }

    //Free surfaces 
	for (int i = 0; i < STORE_SURFACES; i++)
    {
		SDL_FreeSurface(storeStatus->surfaces[i]);
    }

    //Positioner for rects
    storeStatus->headLine_Rect.x = 370;
    storeStatus->headLine_Rect.y = 10;
    SDL_QueryTexture(storeStatus->headLine_Tex, NULL, NULL, &storeStatus->headLine_Rect.w, &storeStatus->headLine_Rect.h);

    storeStatus->bank_Rect.x = 800;
    storeStatus->bank_Rect.y = 10;
    SDL_QueryTexture(storeStatus->bank_Tex, NULL, NULL, &storeStatus->bank_Rect.w, &storeStatus->bank_Rect.h);

    storeStatus->coins_Rect.x = 920;
    storeStatus->coins_Rect.y = 10;
    SDL_QueryTexture(storeStatus->coins_Tex, NULL, NULL, &storeStatus->coins_Rect.w, &storeStatus->coins_Rect.h);

    storeStatus->purchased_Rect.x = 50;
    storeStatus->purchased_Rect.y = 400;
    SDL_QueryTexture(storeStatus->purchased_Tex, NULL, NULL, &storeStatus->purchased_Rect.w, &storeStatus->purchased_Rect.h);

    storeStatus->backToMenu_Rect.x = 300;
    storeStatus->backToMenu_Rect.y = 500;
    SDL_QueryTexture(storeStatus->backToMenu_Tex, NULL, NULL, &storeStatus->backToMenu_Rect.w, &storeStatus->backToMenu_Rect.h);

	for (int i = 0; i < OPTIONS; i++)
    {
        storeStatus->price_Rect[i].x = (73 + a);
        storeStatus->price_Rect[i].y = 355;
        SDL_QueryTexture(storeStatus->price_Tex[i], NULL, NULL, &storeStatus->price_Rect[i].w, &storeStatus->price_Rect[i].h);
        a  = (a + 225);
    }

    //Val av status
    while(!storeStatus->done)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                storeStatus->done = true;
            }
            else if (event.type == SDL_MOUSEMOTION) 
            {
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 1; i < OPTIONS; i++)
                {
                    int compare;
                    compare = strcmp(storeStatus->price[i], storeStatus->purchased);
                    if (compare != 0)
                    {
                        //Om fokus, andra till gron text
                        if (x >= storeStatus->price_Rect[i].x && x <= storeStatus->price_Rect[i].x + storeStatus->price_Rect[i].w && y > storeStatus->price_Rect[i].y && y <= storeStatus->price_Rect[i].y + storeStatus->price_Rect[i].h)
                        {
                            SDL_DestroyTexture(storeStatus->price_Tex[i]);
                            SDL_Surface* temp = TTF_RenderText_Solid(fonts->cuvert_28, storeStatus->price[i], storeStatus->green);
                            storeStatus->price_Tex[i] = SDL_CreateTextureFromSurface(renderer, temp);
                            SDL_FreeSurface(temp);
                        }
                        else
                        {
                            SDL_DestroyTexture(storeStatus->price_Tex[i]);
                            SDL_Surface* temp = TTF_RenderText_Solid(fonts->cuvert_28, storeStatus->price[i], storeStatus->white);
                            storeStatus->price_Tex[i] = SDL_CreateTextureFromSurface(renderer, temp);
                            SDL_FreeSurface(temp);
                        }
                    }
                    if (x >= storeStatus->backToMenu_Rect.x && x <= storeStatus->backToMenu_Rect.x + storeStatus->backToMenu_Rect.w && y > storeStatus->backToMenu_Rect.y && y <= storeStatus->backToMenu_Rect.y + storeStatus->backToMenu_Rect.h)
                    {
                        SDL_DestroyTexture(storeStatus->backToMenu_Tex);
                        SDL_Surface* temp = TTF_RenderText_Solid(fonts->cuvert_48, storeStatus->backToMenu, storeStatus->green);
                        storeStatus->backToMenu_Tex = SDL_CreateTextureFromSurface(renderer, temp);
                        SDL_FreeSurface(temp);
                    }
                    else
                    {
                        SDL_DestroyTexture(storeStatus->backToMenu_Tex);
                        SDL_Surface* tmp = TTF_RenderText_Solid(fonts->cuvert_48, storeStatus->backToMenu, storeStatus->white);
                        storeStatus->backToMenu_Tex = SDL_CreateTextureFromSurface(renderer, tmp);
                        SDL_FreeSurface(tmp);
                    }
                }
                storeStatus->renderText = true;
			}
            //Om knapptryck 
            else if(event.type==SDL_MOUSEBUTTONDOWN)
            {
                x=event.button.x;
                y=event.button.y;
                for (int i = 0; i < OPTIONS; i++)
                {
                    //Tryck pa nagot skin
                    if (x >= storeStatus->price_Rect[i].x && x <= storeStatus->price_Rect[i].x + storeStatus->price_Rect[i].w && y > storeStatus->price_Rect[i].y && y <= storeStatus->price_Rect[i].y + storeStatus->price_Rect[i].h)
                    {
                        //om köp är genomförbart returneras 1
                        if (handlePurchase(storeStatus, i))
                        {
                            strcpy(storeStatus->price[i], storeStatus->purchased);
                            sprintf(storeStatus->coins, "%d", storeStatus->playerCoins);
                            SDL_DestroyTexture(storeStatus->price_Tex[i]);
                            SDL_DestroyTexture(storeStatus->coins_Tex);
                            SDL_Surface* temp = TTF_RenderText_Solid(fonts->cuvert_28, storeStatus->price[i], storeStatus->white);
                            SDL_Surface* tmp = TTF_RenderText_Solid(fonts->cuvert_28, storeStatus->coins, storeStatus->white);
                            storeStatus->price_Tex[i] = SDL_CreateTextureFromSurface(renderer, temp);
                            storeStatus->coins_Tex = SDL_CreateTextureFromSurface(renderer, tmp);
                            SDL_QueryTexture(storeStatus->price_Tex[i], NULL, NULL, &storeStatus->price_Rect[i].w, &storeStatus->price_Rect[i].h);
                            SDL_QueryTexture(storeStatus->coins_Tex, NULL, NULL, &storeStatus->coins_Rect.w, &storeStatus->coins_Rect.h);
                            SDL_FreeSurface(temp);
                            SDL_FreeSurface(tmp);
                            storeStatus->renderText = true;
                        }
                    }
                    else if (x >= storeStatus->skinBackgroundRect[i].x && x <= storeStatus->skinBackgroundRect[i].x + storeStatus->skinBackgroundRect[i].w && y > storeStatus->skinBackgroundRect[i].y && y <= storeStatus->skinBackgroundRect[i].y + storeStatus->skinBackgroundRect[i].h)
                    {
                        if (storeStatus->purchasedSkin[i])
                        {
                            storeStatus->selectedRect = i;
                            switch (i)
                            {
                                case 0: storeStatus->skinChoice = fly;
                                break;
                                case 1: storeStatus->skinChoice = horned;
                                break;
                                case 2: storeStatus->skinChoice = goggle;
                                break;
                                case 3: storeStatus->skinChoice = angry;
                                break;
                            }
                            storeStatus->renderText = true;
                        }
                    }
                    //Tryck pa back to menu
                    else if(x >= storeStatus->backToMenu_Rect.x && x <= storeStatus->backToMenu_Rect.x + storeStatus->backToMenu_Rect.w && y > storeStatus->backToMenu_Rect.y && y <= storeStatus->backToMenu_Rect.y + storeStatus->backToMenu_Rect.h)
                    {
                        storeStatus->renderText = true;
                        storeStatus->done = true;
                    }
                }
            }
        }

        //Rendrar
        if(storeStatus->renderText)
        {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, storeStatus->headLine_Tex, NULL, &storeStatus->headLine_Rect);
            SDL_RenderCopy(renderer, storeStatus->bank_Tex, NULL, &storeStatus->bank_Rect);
            SDL_RenderCopy(renderer, storeStatus->coins_Tex, NULL, &storeStatus->coins_Rect);
            SDL_RenderCopy(renderer, storeStatus->backToMenu_Tex, NULL, &storeStatus->backToMenu_Rect);
            for (int i = 0; i < OPTIONS; i++)
            {
                if (i == storeStatus->selectedRect)
                {
                    SDL_RenderCopy(renderer, media->selectedSkinBackgroundTex, NULL, &storeStatus->skinBackgroundRect[i]);
                }
                else
                {
                    SDL_RenderCopy(renderer, media->skinBackgroundTex, NULL, &storeStatus->skinBackgroundRect[i]);
                }
            }
            SDL_RenderCopy(renderer, media->flyTex, &media->startFlyGreen[1], &storeStatus->skins[0]);
            SDL_RenderCopy(renderer, media->hornedFlyTex, &media->hornedFlyGreen[0], &storeStatus->skins[1]);
            SDL_RenderCopy(renderer, media->goggleEyeFlyTex, &media->goggleEyesFlyGreen[0], &storeStatus->skins[2]);
            SDL_RenderCopy(renderer, media->angryFlyTex, &media->angryFlyGreen[0], &storeStatus->skins[3]);
            for (int i = 0; i < OPTIONS; i++)
            {
                SDL_RenderCopy(renderer, storeStatus->price_Tex[i], NULL, &storeStatus->price_Rect[i]);
            }
            SDL_RenderPresent(renderer);
            storeStatus->renderText = false;
        }
    }

    //Destroy textures
    SDL_DestroyTexture(storeStatus->headLine_Tex);
    SDL_DestroyTexture(storeStatus->bank_Tex);
    SDL_DestroyTexture(storeStatus->coins_Tex);
    SDL_DestroyTexture(storeStatus->purchased_Tex);
    SDL_DestroyTexture(storeStatus->backToMenu_Tex);
    for (int i = 0; i < OPTIONS; i++)
    {
        SDL_DestroyTexture(storeStatus->price_Tex[i]);
    }
}

PRIVATE int handlePurchase(Store storeStatus, int iteration)
{
    switch (iteration)
    {
        case 1:
            if (!storeStatus->purchasedSkin[1])
            {
                if (storeStatus->playerCoins >= 10)
                {
                    storeStatus->playerCoins -= 10;
                    storeStatus->skinChoice = horned;
                    storeStatus->purchasedSkin[1] = true;
                    storeStatus->selectedRect = 1;
                    return 1;
                }
            }
        break;
        case 2:
            if (!storeStatus->purchasedSkin[2])
            {
                if (storeStatus->playerCoins >= 15)
                {
                    storeStatus->playerCoins -= 15;
                    storeStatus->skinChoice = goggle;
                    storeStatus->purchasedSkin[2] = true;
                    storeStatus->selectedRect = 2;
                    return 1;
                }
            }
        break;
        case 3:
            if (!storeStatus->purchasedSkin[3])
            {
                if (storeStatus->playerCoins >= 20)
                {
                    storeStatus->playerCoins -= 20;
                    storeStatus->skinChoice = angry;
                    storeStatus->purchasedSkin[3] = true;
                    storeStatus->selectedRect = 3;
                    return 1;
                }
            }
        break;
    }
    return 0;
}