#include "Network.h"



//Creates Gamestate for sending over network
int start_Game_state(Player playerList[], Game_State current) {

    for (int i=0; current->nrOfPlayers > i; i++){

        current->playerPosX[i] = getPlayerPoint((playerList[i]),'x');
        current->playerPosY[i] = getPlayerPoint((playerList[i]), 'y');
    }

    current->serverConnection = 0;

    return 0;
    //Skapar en Game state struct med startvärden och returnerar den
}

int initGamestate(Game_State current)
{
    for (int i = 0; MAX_PLAYERS > i; i++) {
        current->playerPosX[i] = 0;
        current->playerPosY[i] = 0;
        current->playerAlive[i] = true;
        current->pushAngle[i] = 0;
        current->playerScore[i] = 0;
        current->resurected[i] = false;
        current->playerPower[i] = none;
        current->attackPower = false;
        current->connectionTimers[i] = 0;
        current->playerSkin[i] = fly;
    }
    current->nrOfPlayers = 0;
    current->changeFlag = 0;
    current->obstacleChangeFlag = 0;
    current->powerUpChangeFlag = 0;
    current->lobbyRunningFlag = 1;
    current->newPlayerFlag = 0;
    current->localPlayerNr = 0;
    current->disconnectionCache = 0;
    current->serverConnection = 0;

    return 0;
}


// int netowrk and check what port free
int init_client_network(char playerIp[], UDP_Client_Config setup, Game_State current)
{
    //Öppnar en socket för att ta skicka data
    if (!(setup->send_Sock = (SDLNet_UDP_Open(0))))
    {
        
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    printf("%s\n", playerIp);
 
    //Öppnar en port för att ta emot data om port 2002.
    if (setup->recv_Sock[0] = (SDLNet_UDP_Open(PORT))) //Öppnar en port för att kunna 
    {

        if (SDLNet_ResolveHost(&setup->sendingIP[0], playerIp, PORT) == -1) {

            printf("SDLNet_ResolveHost(%s %d): %s\n", playerIp, PORT, SDLNet_GetError());
            exit(EXIT_FAILURE);
        }
    }
    else {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    //Allokerar plats för två paket
    if (!((setup->send_Pack = SDLNet_AllocPacket(2048)) && (setup->recv_Pack = SDLNet_AllocPacket(2048))))
    {
        printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    return 0;
}

int init_Server_network(UDP_Client_Config setup, Game_State current)
{

    //Öppnar en socket för att ta skicka data
    if (!(setup->send_Sock = (SDLNet_UDP_Open(0))))
    {
        exit(EXIT_FAILURE);
    }

    if (!(setup->recv_Sock[0] = SDLNet_UDP_Open(PORT))) //Öppnar en port för att kunna 
    {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
 
    for (int i = 0; current->nrOfPlayers-1> i; i++)
    {    
            //printf("%s\n", setup->playerIp[i]);

            if (SDLNet_ResolveHost(&setup->sendingIP[i], setup->playerIp[i], PORT) == -1) {

                printf("SDLNet_ResolveHost(%s %d): %s\n", setup->playerIp[i], PORT, SDLNet_GetError());
                exit(EXIT_FAILURE);
            }

    }

    //Allokerar plats för två paket
    if (!((setup->send_Pack = SDLNet_AllocPacket(2048)) && (setup->recv_Pack = SDLNet_AllocPacket(2048))))
    {
        printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    return 0;
}

//sends and recives game data over the network
int sendAndReciveClient(Game_State Gupd, UDP_Client_Config setup, SDL_Rect *playerPos[], Player players[])
{
    updateGameSending(Gupd, playerPos,players);
    networkCommunicationClient(Gupd, setup);
    updateGameReciving(Gupd, playerPos,players);

    return 0;
}

int sendAndReciveServer(Game_State Gupd, UDP_Client_Config setup, SDL_Rect* playerPos[], Player players[])
{
    updateGameSending(Gupd, playerPos, players);
    networkCommunicationServer(Gupd, setup);
    updateGameReciving(Gupd, playerPos, players);

    return 0;
}

//Updates the Gamestate with information that is going to be sent
int updateGameSending(Game_State current, SDL_Rect* playerPos[], Player players[])
{

    SetGameStatePlayerPosX(current, playerPos);
    SetGameStatePlayerPosY(current, playerPos);
    SetGameStatePlayerStatus(current, players);

    return 0;
}

int updateGameReciving(Game_State current, SDL_Rect* playerPos[], Player players[])
{
    for (int i = 0; current->nrOfPlayers> i; i++)
    {

        if (current->localPlayerNr-1 != i) {
            playerPos[i]->x = current->playerPosX[i];
            playerPos[i]->y = current->playerPosY[i];
            setPlayerStatus(players[i], current->playerAlive[i]);
            setPlayerResurect(players[i], current->resurected[i]);
            setPlayerPower(players[i], current->playerPower[i]);
            setPlayerSkin(players[i], current->playerSkin[i]);
            setPlayerAttack(players[i], current->attack[i]);
        }
    }
    return 0;
}

//
int networkCommunicationClient(Game_State current, UDP_Client_Config setup)
{

    //Om flaggan är satt så finns det ny data att skicka
    if (current->changeFlag == 1) {

        Game_State_Send Gupd_Sending = malloc(sizeof(struct Game_State_Send_Type));

        Gupd_Sending->playerPosX = current->playerPosX[current->localPlayerNr - 1];
        Gupd_Sending->playerPosY = current->playerPosY[current->localPlayerNr - 1];
        Gupd_Sending->playerAlive = current->playerAlive[current->localPlayerNr - 1];
        Gupd_Sending->playerScore = current->playerScore[current->localPlayerNr - 1];
        Gupd_Sending->resurected = current->resurected[current->localPlayerNr - 1];
        Gupd_Sending->playerPower = current->playerPower[current->localPlayerNr - 1];
        Gupd_Sending->playerSkin = current->playerSkin[current->localPlayerNr - 1];
        Gupd_Sending->attack = current->attack[current->localPlayerNr - 1];
        Gupd_Sending->localPlayerNr = current->localPlayerNr;


        for (int i = 0; current->nrOfPlayers > i; i++) {
            
            if (current->localPlayerNr - 1 != i) {
                Gupd_Sending->pushAngle[i] = current->pushAngle[i];
                if (current->playerPower[i] == attack) {
                    current->attackPower = true;
                }
                else {
                    current->attackPower = false;
                }
                current->pushAngle[i] = 0;
            }
            else {
                Gupd_Sending->pushAngle[i] = 0;
            }
        }

        //printf("Client Sending: %d\n", Gupd_Sending->player_Pos_Y);

        memcpy(setup->send_Pack->data, Gupd_Sending, sizeof(struct Game_State_Send_Type)); //kopierar Game_state till paketet
        setup->send_Pack->len = sizeof(struct Game_State_Send_Type);
        setup->send_Pack->address.host = setup->sendingIP[0].host;	
        setup->send_Pack->address.port = setup->sendingIP[0].port;	
        SDLNet_UDP_Send(setup->send_Sock, -1, setup->send_Pack);

        current->changeFlag = 0;
        current->obstacleChangeFlag = 0;
        current->powerUpChangeFlag = 0;

        free(Gupd_Sending);
    }

    current->connectionTimers[0]++;

    //kollar om det finns ett packet att hämta
    if (SDLNet_UDP_Recv(setup->recv_Sock[0], setup->recv_Pack)) {

        current->connectionTimers[0] = 0;

        Game_State Gupd_Recive = malloc(sizeof(struct Game_State_Type));

        initGamestate(Gupd_Recive);
      
        memcpy(Gupd_Recive, setup->recv_Pack->data, sizeof(struct Game_State_Type));

        for (int i=0; i < current->nrOfPlayers; i++) {
            if (current->localPlayerNr-1 != i) {
                current->playerPosX[i] = Gupd_Recive->playerPosX[i];
                current->playerPosY[i] = Gupd_Recive->playerPosY[i];
                current->playerAlive[i] = Gupd_Recive->playerAlive[i];
                current->playerScore[i] = Gupd_Recive->playerScore[i];
                current->resurected[i] = Gupd_Recive->resurected[i];
                current->playerPower[i] = Gupd_Recive->playerPower[i];
                current->playerSkin[i] = Gupd_Recive->playerSkin[i];
                current->attack[i] = Gupd_Recive->attack[i];

            }

            if (Gupd_Recive->pushAngle[current->localPlayerNr - 1] != 0) {
                current->pushAngle[current->localPlayerNr - 1] = Gupd_Recive->pushAngle[current->localPlayerNr - 1];
            }
            


            if (Gupd_Recive->pushAngle[i] != 0)
            {
                printf("Angle: %d Player: %d\n", Gupd_Recive->pushAngle[i], i);
            }
        }

        //printf("Reciving: %d %d\n", (int)Gupd_Recive->player_Pos_X[0], (int)Gupd_Recive->player_Pos_Y[0]);

        current->obstacle_bottom = Gupd_Recive->obstacle_bottom;
        current->obstacle_top = Gupd_Recive->obstacle_top;
        current->obstacleChangeFlag = Gupd_Recive->obstacleChangeFlag;

        for (int i = 0; current->nrOfPlayers > i; i++) {
            if (current->localPlayerNr - 1 != i)
                current->pushAngle[i] = 0;
        }

        
        current->powerUpRect = Gupd_Recive->powerUpRect;
        current->powerUpDir = Gupd_Recive->powerUpDir;
        current->powerUpType = Gupd_Recive->powerUpType;
        current->powerUpChangeFlag = Gupd_Recive->powerUpChangeFlag;

        free(Gupd_Recive);
    }

    return 0;
}


int networkCommunicationServer(Game_State current, UDP_Client_Config setup)
{
    for (int i = 0; current->nrOfPlayers - 1 > i; i++) {
        current->connectionTimers[i]++;
    }


    for (int i = 0; current->nrOfPlayers - 1 > i; i++)
    {

        if (SDLNet_UDP_Recv(setup->recv_Sock[0], setup->recv_Pack)) {

            current->changeFlag = 1;

            Game_State_Send Gupd_Recive = malloc(sizeof(struct Game_State_Send_Type));
            memcpy(Gupd_Recive, setup->recv_Pack->data, sizeof(struct Game_State_Send_Type));

            current->connectionTimers[Gupd_Recive->localPlayerNr] = 0;

            
            current->playerPosX[Gupd_Recive->localPlayerNr - 1] = Gupd_Recive->playerPosX;
            current->playerPosY[Gupd_Recive->localPlayerNr - 1] = Gupd_Recive->playerPosY;
            current->playerAlive[Gupd_Recive->localPlayerNr - 1] = Gupd_Recive->playerAlive;
            current->playerScore[Gupd_Recive->localPlayerNr - 1] = Gupd_Recive->playerScore;
            current->resurected[Gupd_Recive->localPlayerNr - 1] = Gupd_Recive->resurected;
            current->playerPower[Gupd_Recive->localPlayerNr - 1] = Gupd_Recive->playerPower;
            current->playerSkin[Gupd_Recive->localPlayerNr - 1] = Gupd_Recive->playerSkin;
            current->attack[Gupd_Recive->localPlayerNr - 1] = Gupd_Recive->attack;

            for (int i = 0; current->nrOfPlayers > i; i++) {
                if (Gupd_Recive->pushAngle[i] != 0)
                {
                    current->pushAngle[i] = Gupd_Recive->pushAngle[i];
                    if(Gupd_Recive->playerPower == attack){
                        current->attackPower = true;
                    }
                    else {
                        current->attackPower = false;
                    }
                    printf("Angle: %d Player: %d\n", Gupd_Recive->pushAngle[i],i);
                }
            }


            free(Gupd_Recive);
        }
    }


    //Om flaggan är satt så finns det ny data att skicka
    if (current->changeFlag == 1) {

        memcpy(setup->send_Pack->data, current, sizeof(struct Game_State_Type)); //kopierar Game_state till paketet
        setup->send_Pack->len = sizeof(struct Game_State_Type);

        for (int i = 0; current->nrOfPlayers > i; i++) {
            setup->send_Pack->address.host = setup->sendingIP[i].host;
            setup->send_Pack->address.port = setup->sendingIP[i].port;
            SDLNet_UDP_Send(setup->send_Sock, -1, setup->send_Pack);
        }

        current->changeFlag = 0;
        current->obstacleChangeFlag = 0;
        for (int i = 0; current->nrOfPlayers > i; i++) {
            if(current->localPlayerNr-1!=i)
                current->pushAngle[i] = 0;
        }
        current->powerUpChangeFlag = 0;
    }   

    //kollar om det finns ett packet att hämta

    return 0;
}


int serverLobbyConnection(Game_State current)
{

    IPaddress ip1;
    IPaddress *ipRecive;
    TCP_Communication communication = malloc(sizeof(struct TCP_Communication_Type));

    initTCPCom(communication);

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

     SDLNet_ResolveHost(&ip1, NULL, PORT);

     TCPsocket server = SDLNet_TCP_Open(&ip1);
     TCPsocket client;
       
     do {

         client = SDLNet_TCP_Accept(server);


         if (client)
         {
             
            SDLNet_TCP_Recv(client, communication, sizeof(struct TCP_Communication_Type)); //Tar emot namnet som skickas över strömmen

            if (communication->leftGame) {
                current->disconnectionCache = communication->localPlayerNr;
                communication->leftGame = 0;
            }
            else {

                ipRecive = SDLNet_TCP_GetPeerAddress(client); //Väntar tills en klient kopplar upp sig och tar IP:n från TCP strömmen

                         //current->ipAdressCache = SDLNet_ResolveIP(&ip_Recive); //Test snabbare koppling

                strncpy(current->ipAdressCache, SDLNet_ResolveIP(ipRecive), IP_LENGTH);
                strncpy(current->playerNames[current->nrOfPlayers], communication->playerName, NAME_LENGTH);
                current->nrOfPlayers++;
                current->newPlayerFlag = 1;



                for (int i = 0; current->nrOfPlayers - 1 > i; i++) {
                    strncpy(communication->playerName, current->playerNames[i], NAME_LENGTH);
                    if (current->nrOfPlayers - 2 == i) {
                        printf("Sending close\n");
                        communication->connectionOpen = 0;
                    }
                    communication->recived = 1;
                    SDLNet_TCP_Send(client, communication, sizeof(struct TCP_Communication_Type));
                    SDL_Delay(150);
                }
            }
         }
     } while (current->lobbyRunningFlag);


     free(communication);
     SDLNet_TCP_Close(server);
     SDLNet_Quit();

    return 0;
}



int clientLobbyConnection(char playerIp[], char playerName[], Game_State current)
{
    int timer = 0;
    int connectionTime = 1;
    TCPsocket server;
    IPaddress ip1;
    TCP_Communication communication = malloc(sizeof(struct TCP_Communication_Type));

    initTCPCom(communication);
    strncpy(communication->playerName, playerName, NAME_LENGTH);

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    SDLNet_ResolveHost(&ip1, playerIp, PORT);

    do {
        server = SDLNet_TCP_Open(&ip1);
        timer++;
    } while (server == NULL&& timer!=connectionTime);
   
    if (timer == connectionTime) {
        return 1;
    }

    SDLNet_TCP_Send(server, communication, sizeof(struct TCP_Communication_Type));

    do {    
            SDLNet_TCP_Recv(server, communication, sizeof(struct TCP_Communication_Type));

            if (communication->recived) {
                
                strcpy(current->playerNames[current->nrOfPlayers], communication->playerName);
                current->nrOfPlayers++;
                current->localPlayerNr++;
                communication->recived = 1;
            }
            
    } while (communication->connectionOpen);


    strcpy(current->playerNames[current->nrOfPlayers], playerName);
    current->nrOfPlayers++;
    current->localPlayerNr++;

    free(communication);
    SDLNet_TCP_Close(server);

    return 0;
}

int sendToClient(TCP_Communication communication, char playerIp[], Game_State current)
{
    TCPsocket server;
    IPaddress ip1;

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    SDLNet_ResolveHost(&ip1, playerIp, PORT);
    do {
        server = SDLNet_TCP_Open(&ip1);
    } while (server == NULL);

    SDLNet_TCP_Send(server, communication, sizeof(struct TCP_Communication_Type));

    SDLNet_TCP_Close(server);

    return 0;
}

int clientLobbyWait(Game_State current)
{
    IPaddress ip1;
    TCP_Communication communication = malloc(sizeof(struct TCP_Communication_Type));

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    initTCPCom(communication);
    SDLNet_ResolveHost(&ip1, NULL, PORT);

    TCPsocket server = SDLNet_TCP_Open(&ip1);
    TCPsocket client;

    do {

        client = SDLNet_TCP_Accept(server);

        if (client)
        {
                SDLNet_TCP_Recv(client, communication, sizeof(struct TCP_Communication_Type)); //Tar emot namnet som skickas över strömmen

                if (communication->startGame) {
                    current->lobbyRunningFlag = 0;
                }
                else if (communication->serverDisconnect) {
                    current->serverConnection = 1;
                }
                else if(communication->leftGame){
                    communication->leftGame = 0;
                    current->disconnectionCache = communication->localPlayerNr;
                }else{
                    strcpy(current->playerNames[current->nrOfPlayers], communication->playerName);
                    current->nrOfPlayers++;
                    current->newPlayerFlag = 1;
                }
        }

    } while (current->lobbyRunningFlag);

    free(communication);
    SDLNet_TCP_Close(server);
    SDLNet_Quit();

    return 0; 
}

int disconnectFromServer(char playerIp[], Game_State current)
{
    TCPsocket server;
    IPaddress ip1;
    TCP_Communication communication = malloc(sizeof(struct TCP_Communication_Type));

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    SDLNet_ResolveHost(&ip1, playerIp, PORT);
    do {
        server = SDLNet_TCP_Open(&ip1);
    } while (server == NULL);

    communication->localPlayerNr = current->localPlayerNr;
    communication->leftGame = 1;
    current->nrOfPlayers = 0;
    current->lobbyRunningFlag = 0;


    SDLNet_TCP_Send(server, communication, sizeof(struct TCP_Communication_Type));

    SDLNet_TCP_Close(server);

    return 0;
}

int closeServer(Game_State current, TCP_Communication communication, UDP_Client_Config setup)
{
    communication->serverDisconnect = 1;
    for (int i = 0; current->nrOfPlayers - 1 > i; i++) {
        sendToClient(communication, setup->playerIp[i], current);
        printf("Server closing\n");
    }
    current->lobbyRunningFlag = 0;
    current->nrOfPlayers = 0;

    return 0; 
}

//Stänger ner och rensar
int Close_SDLNet(UDP_Client_Config setup, Game_State current)
{
    SDLNet_FreePacket(setup->send_Pack);
    SDLNet_FreePacket(setup->recv_Pack);
    free(setup);
    free(current);
    SDLNet_Quit();

    return 0;
}

int resetClientSDLNet(UDP_Client_Config setup)
{
    SDLNet_FreePacket(setup->send_Pack);
    SDLNet_FreePacket(setup->recv_Pack);
    SDLNet_UDP_Close(setup->recv_Sock[0]);
    return 0;
}

int resetServerSDLNet(UDP_Client_Config setup, Game_State current)
{
    SDLNet_FreePacket(setup->send_Pack);
    SDLNet_FreePacket(setup->recv_Pack);

    for(int i = 0; current->nrOfPlayers-1>i;i++)
        SDLNet_UDP_Close(setup->recv_Sock[0]);
    return 0;
}

int getGameStatePlayerPosX(Game_State current, int playerNr)
{
    return current->playerPosX[playerNr];
}

int getGameStatePlayerPosY(Game_State current, int playerNr)
{
    return current->playerPosY[playerNr];

}

int SetGameStatePlayerPosX(Game_State current, SDL_Rect* playerPos[])
{
    
    if (current->playerPosX[current->localPlayerNr-1] != playerPos[current->localPlayerNr-1]->x) {

        current->changeFlag = 1;
        current->playerPosX[current->localPlayerNr-1] = playerPos[current->localPlayerNr-1]->x;
    }
    return 0;
}

int SetGameStatePlayerPosY(Game_State current, SDL_Rect* playerPos[])
{

    if (current->playerPosY[current->localPlayerNr-1] != playerPos[current->localPlayerNr-1]->y) {

        current->changeFlag = 1;
        current->playerPosY[current->localPlayerNr-1] = playerPos[current->localPlayerNr-1]->y;
    }
    return 0;
}

int SetGameStatePlayerStatus(Game_State current, Player players[])
{
    if (current->playerAlive[current->localPlayerNr-1] != getPlayerStatus(players[current->localPlayerNr-1]) ) {

        current->changeFlag = 1;
        current->playerAlive[current->localPlayerNr-1] = getPlayerStatus(players[current->localPlayerNr-1]);
    }

    if (current->playerScore[current->localPlayerNr - 1] != getPlayerScore(players[current->localPlayerNr - 1])) {

        current->changeFlag = 1;
        current->playerScore[current->localPlayerNr - 1] = getPlayerScore(players[current->localPlayerNr - 1]);
    }

    if (current->resurected[current->localPlayerNr - 1] != getPlayerResurect(players[current->localPlayerNr - 1])) {

        current->changeFlag = 1;
        current->resurected[current->localPlayerNr - 1] = getPlayerResurect(players[current->localPlayerNr - 1]);
    }

    if (current->playerPower[current->localPlayerNr - 1] != getPlayerPower(players[current->localPlayerNr - 1])) {

        current->changeFlag = 1;
        current->playerPower[current->localPlayerNr - 1] = getPlayerPower(players[current->localPlayerNr - 1]);
    }


    if (current->playerSkin[current->localPlayerNr - 1] != getPlayerSkin(players[current->localPlayerNr - 1])) {

        current->changeFlag = 1;
        current->playerSkin[current->localPlayerNr - 1] = getPlayerSkin(players[current->localPlayerNr - 1]);
    }

    if (current->attack[current->localPlayerNr - 1] != getPlayerAttack(players[current->localPlayerNr - 1])) {

        current->changeFlag = 1;
        current->attack[current->localPlayerNr - 1] = getPlayerAttack(players[current->localPlayerNr - 1]);
    }

    return 0;
}

//Uppdaterar obstacle i Game_state
int SetObstacle(Game_State Gupd, Obstacle Send_obstacles)
{
    Gupd->changeFlag = 1; //sätter change flaggan så att det skickas över nätverket
    Gupd->obstacleChangeFlag = 1; //obstacle flaggan visar för klienten att ett nytt obstacle finns att rendra
    Gupd->obstacle_top = getRectfromObstacle(getNextObsFromList(Send_obstacles), true);
    Gupd->obstacle_bottom = getRectfromObstacle(getNextObsFromList(Send_obstacles), false);
    
    return 0;
}

Obstacle ReciveObstacle(Game_State Gupd)
{
    Obstacle recive_obstacle = createClientObstacle(Gupd->obstacle_top, Gupd->obstacle_bottom);

    Gupd->obstacleChangeFlag = 0;

    return recive_obstacle;
}

void SetPowerUp(Game_State current, PowerUp aPowerUp) {
    current->changeFlag = 1; //sätter change flaggan så att det skickas över nätverket
    current->powerUpChangeFlag = 1; //powerup flaggan visar för klienten att en ny powerup genererats
    current->powerUpRect = getPowerUpRect(aPowerUp); //kopierar över powerups attribut
    current->powerUpDir = getPowerUpDir(aPowerUp); //kopierar över powerups attribut
    current->powerUpType = getPowerUpType(aPowerUp); //kopierar över powerups attribut
}

PowerUp ReceivePowerUp(Game_State current) {
    PowerUp newPowerUp = clientSpawnPowerUp(current->powerUpRect, current->powerUpDir, current->powerUpType);
    current->powerUpChangeFlag = 0;
    return newPowerUp;
}

void renderConnectionsServer(Game_State current) {

    for (int i = 1; current->nrOfPlayers - 1 > i; i++) {
        
        if (current->connectionTimers[i] == DISCONNECT_TIMER && current->playerAlive[i] != 0) {
            current->playerAlive[i] = 0;
            printf("Player: %s disconnected\n", current->playerNames[i]);
        }
    }

}

int renderConnectionsClient(Game_State current) {

        if (current->connectionTimers[0] == DISCONNECT_TIMER) {
            printf("Player(Host): %s disconnected\n", current->playerNames[0]);
            return 1;
        }

        return 0;
}

void removePlayerFromLobby(Game_State current, UDP_Client_Config setup, int localPlayerNr){

    if (current->nrOfPlayers > 2 && localPlayerNr != current->nrOfPlayers) {
            for (int i = localPlayerNr - 1; current->nrOfPlayers > i; i++) {
                strncpy(current->playerNames[i], current->playerNames[i + 1], NAME_LENGTH);
                if (setup->playerIp[0] != NULL&& current->nrOfPlayers-1 > i) {
                    strncpy(setup->playerIp[i-1], setup->playerIp[i], IP_LENGTH);
                }
                
            }
    }

    strcpy(current->playerNames[current->nrOfPlayers - 1], "*empty*");
    current->nrOfPlayers = current->nrOfPlayers - 1;
}

int initTCPCom(TCP_Communication communication) {
    communication->leftGame = 0;
    communication->connectionOpen = 1;
    communication->startGame = 0;
    communication->recived = 0;
    communication->localPlayerNr = 0;
    communication->serverDisconnect = 0;

    return 0;
}

