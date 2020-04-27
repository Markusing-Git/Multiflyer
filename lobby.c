#include "lobby.h"

typedef struct {
	char lobbyText[NAME_LENGTH];
	char startGame[NAME_LENGTH];
	char waitingForHost[NAME_LENGTH];
	char slots[4][8];
	char playerNames[4][NAME_LENGTH];
	bool running;
	int playerCount;
	bool renderText;

	SDL_Texture* textures[TEXTS];
	SDL_Texture* startGameTexture;
	SDL_Texture* waitingForHostTex;
	SDL_Rect rects[TEXTS];
	SDL_Rect startGameRect;
	SDL_Rect waitingForHostRect;
	Player players[MAX_PLAYERS];
	SDL_Event event;

	TTF_Font* headLine;
	TTF_Font* playerList;
	TTF_Font* startGameFont;
	SDL_Color lobbyTextColor;
	SDL_Color playerListColor;

	SDL_Surface* surfaces[11];

}Lobby;


PRIVATE void renderLobby(SDL_Renderer* renderer, bool hostOrClient, Lobby aLobby);
PRIVATE void closeLobbyTTF(Lobby aLobby);
PRIVATE Lobby createLobby(SDL_Renderer* renderer);



// ************************************************** HOST CODE ******************************************************************************************************
PUBLIC int hostLobby(SDL_Renderer* renderer,Game_State current, char playerIp[]) {

	Lobby hostLobby;
	hostLobby = createLobby(renderer);
	int x, y;

	SDL_Color selected = { 77 , 255, 0, 0 };

	while (hostLobby.running) {

		//serverLobbyConnection(playerIp, current, hostLobby);

		while (SDL_PollEvent(&hostLobby.event))
		{

			if (hostLobby.event.type == SDL_QUIT || hostLobby.event.key.keysym.sym == SDLK_ESCAPE)
			{
				closeLobbyTTF(hostLobby);
				hostLobby.running = false;
				return 0;
			}
			else if (hostLobby.event.type == SDL_MOUSEMOTION) {
				x = hostLobby.event.motion.x;
				y = hostLobby.event.motion.y;
				if (x >= hostLobby.startGameRect.x && x <= hostLobby.startGameRect.x + hostLobby.startGameRect.w && y > hostLobby.startGameRect.y && y <= hostLobby.startGameRect.y + hostLobby.startGameRect.h)
				{

					SDL_Surface* temp = TTF_RenderText_Solid(hostLobby.startGameFont, hostLobby.startGame, selected);
					hostLobby.startGameTexture = SDL_CreateTextureFromSurface(renderer, temp);
					SDL_FreeSurface(temp);
					hostLobby.renderText = true;
				}
				else
				{
					SDL_Surface* temp = TTF_RenderText_Solid(hostLobby.startGameFont, hostLobby.startGame, hostLobby.lobbyTextColor);
					hostLobby.startGameTexture = SDL_CreateTextureFromSurface(renderer, temp);
					SDL_FreeSurface(temp);
					hostLobby.renderText = true;
				}
			}
			else if (hostLobby.event.type == SDL_MOUSEBUTTONDOWN)
			{
				x = hostLobby.event.button.x;
				y = hostLobby.event.button.y;
				if (x >= hostLobby.startGameRect.x && x <= hostLobby.startGameRect.x + hostLobby.startGameRect.w && y > hostLobby.startGameRect.y && y <= hostLobby.startGameRect.y + hostLobby.startGameRect.h)
				{
					closeLobbyTTF(hostLobby);
					hostLobby.running = false;
					return 1;
				}
			}
		}
		if (hostLobby.renderText) {
			renderLobby(renderer, true, hostLobby);
		}
	}
}



// ************************************************** CLIENT CODE ****************************************************************************************************
PUBLIC void clientLobby(SDL_Renderer* renderer, char playerName[], char playerIp[]) {

	Lobby clientLobby;
	clientLobby = createLobby(renderer);
	int loadingCounter = 50;
	int loadingDots = 3;

	//clientLobbyConnection(playerIp, playerName);

	while (clientLobby.running) {

		//makes dots on waiting for host count down
		if (loadingCounter <= 0) {
			char* p = clientLobby.waitingForHost;
			p++[strlen(p) - 1] = 0;
			if (loadingDots <= 0) {
				strcpy(clientLobby.waitingForHost, "Waiting for host...");
				loadingDots = 3;
			}
			else {
				loadingDots--;
			}
			SDL_Surface* temp = TTF_RenderText_Solid(clientLobby.headLine, clientLobby.waitingForHost, clientLobby.lobbyTextColor);
			clientLobby.waitingForHostTex = SDL_CreateTextureFromSurface(renderer, temp);
			SDL_QueryTexture(clientLobby.waitingForHostTex, NULL, NULL, &clientLobby.waitingForHostRect.w, &clientLobby.waitingForHostRect.h);
			SDL_FreeSurface(temp);
			clientLobby.renderText = true;
			loadingCounter = 50;
		}
		loadingCounter--;

		while (SDL_PollEvent(&clientLobby.event))
		{
			if (clientLobby.event.type == SDL_QUIT || clientLobby.event.key.keysym.sym == SDLK_ESCAPE)
			{
				closeLobbyTTF(clientLobby);
				clientLobby.running = false;
				return 0;
			}
		}
		if (clientLobby.renderText) {
			renderLobby(renderer, false, clientLobby);
		}
	}
	closeLobbyTTF(clientLobby);
}


// ************************************************** PRIVATE LOBBY FUNCTIONS *****************************************************************************************
PRIVATE Lobby createLobby(SDL_Renderer* renderer) {
	Lobby newLobby;

	strcpy(newLobby.lobbyText, "Lobby");
	strcpy(newLobby.startGame, "Start Game");
	strcpy(newLobby.waitingForHost, "Waiting for host...");
	strcpy(newLobby.slots[0], "slot 1:");
	strcpy(newLobby.slots[1], "slot 2:");
	strcpy(newLobby.slots[2], "slot 3:");
	strcpy(newLobby.slots[3], "slot 4:");

	strcpy(newLobby.playerNames[0], "*empty*");
	strcpy(newLobby.playerNames[1], "*empty*");
	strcpy(newLobby.playerNames[2], "*empty*");
	strcpy(newLobby.playerNames[3], "*empty*");

	newLobby.renderText = true;
	newLobby.running = true;
	newLobby.playerCount = 0;


	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
	}
	newLobby.headLine = TTF_OpenFont("fonts/Curvert.otf", 48);
	if (newLobby.headLine == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
	}
	newLobby.playerList = TTF_OpenFont("fonts/Curvert.otf", 28);
	if (newLobby.playerList == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
	}
	newLobby.startGameFont = TTF_OpenFont("fonts/ka1.ttf", 60);
	if (newLobby.startGameFont == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
	}

	newLobby.lobbyTextColor.r = 255;
	newLobby.lobbyTextColor.g = 255;
	newLobby.lobbyTextColor.b = 255;
	newLobby.lobbyTextColor.a = 0;

	newLobby.playerListColor.r = 160;
	newLobby.playerListColor.g = 237;
	newLobby.playerListColor.b = 189;
	newLobby.playerListColor.a = 0;

	//create surfaces from TTF
	newLobby.surfaces[0] = TTF_RenderText_Solid(newLobby.headLine, newLobby.lobbyText, newLobby.lobbyTextColor);

	for (int i = 1; i < 5; i++)
		newLobby.surfaces[i] = TTF_RenderText_Solid(newLobby.playerList, newLobby.slots[i - 1], newLobby.playerListColor);

	for (int i = 5; i < 9; i++)
		newLobby.surfaces[i] = TTF_RenderText_Solid(newLobby.playerList, newLobby.playerNames[i - 5], newLobby.playerListColor);

	newLobby.surfaces[9] = TTF_RenderText_Solid(newLobby.startGameFont, newLobby.startGame, newLobby.lobbyTextColor);
	newLobby.surfaces[10] = TTF_RenderText_Solid(newLobby.headLine, newLobby.waitingForHost, newLobby.lobbyTextColor);

	//create textures from surfaces
	for (int i = 0; i < 9; i++)
		newLobby.textures[i] = SDL_CreateTextureFromSurface(renderer, newLobby.surfaces[i]);

	newLobby.startGameTexture = SDL_CreateTextureFromSurface(renderer, newLobby.surfaces[9]);
	newLobby.waitingForHostTex = SDL_CreateTextureFromSurface(renderer, newLobby.surfaces[10]);

	//destroy unnessesary surfaces 
	for (int i = 0; i < 11; i++)
		SDL_FreeSurface(newLobby.surfaces[i]);

	//lobby rect
	newLobby.rects[0].x = 390;
	newLobby.rects[0].y = 10;

	//slot rects
	newLobby.rects[1].x = 100;
	newLobby.rects[1].y = 100;
	newLobby.rects[2].x = 100;
	newLobby.rects[2].y = 200;
	newLobby.rects[3].x = 100;
	newLobby.rects[3].y = 300;
	newLobby.rects[4].x = 100;
	newLobby.rects[4].y = 400;

	//playerlist rects
	newLobby.rects[5].x = 280;
	newLobby.rects[5].y = 100;
	newLobby.rects[6].x = 280;
	newLobby.rects[6].y = 200;
	newLobby.rects[7].x = 280;
	newLobby.rects[7].y = 300;
	newLobby.rects[8].x = 280;
	newLobby.rects[8].y = 400;


	newLobby.startGameRect.x = 250;
	newLobby.startGameRect.y = 490;
	SDL_QueryTexture(newLobby.startGameTexture, NULL, NULL, &newLobby.startGameRect.w, &newLobby.startGameRect.h);

	newLobby.waitingForHostRect.x = 230;
	newLobby.waitingForHostRect.y = 490;
	SDL_QueryTexture(newLobby.waitingForHostTex, NULL, NULL, &newLobby.waitingForHostRect.w, &newLobby.waitingForHostRect.h);

	for (int i = 0; i < TEXTS; i++) {
		SDL_QueryTexture(newLobby.textures[i], NULL, NULL, &newLobby.rects[i].w, &newLobby.rects[i].h);
	}

	return newLobby;
}




PRIVATE void renderLobby(SDL_Renderer* renderer, bool hostOrClient, Lobby aLobby) {
	SDL_RenderClear(renderer);

	if (hostOrClient) {
		SDL_RenderCopy(renderer, aLobby.startGameTexture, NULL, &aLobby.startGameRect);
	}
	else {
		SDL_RenderCopy(renderer, aLobby.waitingForHostTex, NULL, &aLobby.waitingForHostRect);
	}

	for (int i = 0; i < TEXTS; i++) {
		SDL_RenderCopy(renderer, aLobby.textures[i], NULL, &aLobby.rects[i]);
	}

	SDL_RenderPresent(renderer);
	aLobby.renderText = false;
}



PRIVATE void closeLobbyTTF(Lobby aLobby) {
	for (int i = 0; i < TEXTS; i++)
		SDL_DestroyTexture(aLobby.textures[i]);
	SDL_DestroyTexture(aLobby.startGameTexture);
	TTF_CloseFont(aLobby.headLine);
	TTF_CloseFont(aLobby.playerList);
	TTF_CloseFont(aLobby.startGameFont);
	TTF_Quit();
}
