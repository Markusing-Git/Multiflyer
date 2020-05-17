#include "Network.h"



//Creates Gamestate for sending over network
int start_Game_state(Player playerList[], Game_State current) {

    for (int i=0; current->nrOfPlayers > i; i++){

        current->player_Pos_X[i] = getPlayerPoint((playerList[i]),'x');
        current->player_Pos_Y[i] = getPlayerPoint((playerList[i]), 'y');
    }
    return 0;
    //Skapar en Game state struct med startvärden och returnerar den
}

int initGamestate(Game_State current)
{
    for (int i = 0; MAX_PLAYERS > i; i++) {
        current->player_Pos_X[i] = 0;
        current->player_Pos_Y[i] = 0;
        current->player_Alive[i] = true;
        current->pushAngle[i] = 0;
        current->playerScore[i] = 0;
        current->connectionTimers[i] = 0;
    }
    current->nrOfPlayers = 0;
    current->change_flag = 0;
    current->obstacle_change_flag = 0;
    current->powerUp_change_flag = 0;
    current->lobbyRunningFlag = 1;
    current->newPlayerFlag = 0;
    current->localPlayerNr = 0;

    return 0;
}


// int netowrk and check what port free
int init_client_network(char playerIp[], UDP_Client_Config setup, Game_State current)
{
 
    int sendPort;
    int recvPort;

    sendPort = 2002;
    recvPort = 2002;


    //Öppnar en socket för att ta skicka data
    if (!(setup->send_Sock = (SDLNet_UDP_Open(0))))
    {
        
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    printf("%s\n", playerIp);

    printf("Client sendport: %d\n", sendPort);
    printf("Client recvport: %d\n", recvPort);
   
    //Öppnar en port för att ta emot data om port 2000 är upptagen och tar den 2001.
    if (setup->recv_Sock[0] = (SDLNet_UDP_Open(recvPort))) //Öppnar en port för att kunna 
    {
        setup->port[0] = recvPort;

        if (SDLNet_ResolveHost(&setup->sendingIP[0], playerIp, sendPort) == -1) {

            printf("SDLNet_ResolveHost(%s %d): %s\n", playerIp, sendPort, SDLNet_GetError());
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

    int sendPort = 2002;
    int recvPort = 2002;

    //Öppnar en socket för att ta skicka data
    if (!(setup->send_Sock = (SDLNet_UDP_Open(0))))
    {
        exit(EXIT_FAILURE);
    }

    if (setup->recv_Sock[0] = (SDLNet_UDP_Open(recvPort))) //Öppnar en port för att kunna 
    {
        setup->port[0] = recvPort + current->localPlayerNr;
    }
    else {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
 
    for (int i = 0; current->nrOfPlayers-1> i; i++)
    {    

       
            setup->port[i] = recvPort + current->localPlayerNr;

            printf("Server sendPort: %d\n", sendPort);
            printf("Server recvPort: %d\n", recvPort);
            //printf("%s\n", setup->playerIp[i]);

            if (SDLNet_ResolveHost(&setup->sendingIP[i], setup->playerIp[i], sendPort) == -1) {

                printf("SDLNet_ResolveHost(%s %d): %s\n", setup->playerIp[i], sendPort, SDLNet_GetError());
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
            playerPos[i]->x = current->player_Pos_X[i];
            playerPos[i]->y = current->player_Pos_Y[i];
            setPlayerStatus(players[i], current->player_Alive[i]);
        }
    }
    return 0;
}

//
int networkCommunicationClient(Game_State current, UDP_Client_Config setup)
{

    //Om flaggan är satt så finns det ny data att skicka
    if (current->change_flag == 1) {

        Game_State_Send Gupd_Sending = malloc(sizeof(struct Game_State_Send_Type));

        Gupd_Sending->player_Pos_X = current->player_Pos_X[current->localPlayerNr - 1];
        Gupd_Sending->player_Pos_Y = current->player_Pos_Y[current->localPlayerNr - 1];
        Gupd_Sending->player_Alive = current->player_Alive[current->localPlayerNr - 1];
        Gupd_Sending->playerScore = current->playerScore[current->localPlayerNr - 1];
        Gupd_Sending->localPlayerNr = current->localPlayerNr;

        for (int i = 0; current->nrOfPlayers > i; i++) {
            Gupd_Sending->pushAngle[i] = current->pushAngle[i];
            current->pushAngle[i] = 0;
        }

        //printf("Client Sending: %d\n", Gupd_Sending->player_Pos_Y);

        memcpy(setup->send_Pack->data, Gupd_Sending, sizeof(struct Game_State_Send_Type)); //kopierar Game_state till paketet
        setup->send_Pack->len = sizeof(struct Game_State_Send_Type);
        setup->send_Pack->address.host = setup->sendingIP[0].host;	
        setup->send_Pack->address.port = setup->sendingIP[0].port;	
        SDLNet_UDP_Send(setup->send_Sock, -1, setup->send_Pack);

        current->change_flag = 0;
        current->obstacle_change_flag = 0;
        current->powerUp_change_flag = 0;

        free(Gupd_Sending);
    }

    //kollar om det finns ett packet att hämta
    if (SDLNet_UDP_Recv(setup->recv_Sock[0], setup->recv_Pack)) {

        Game_State Gupd_Recive = malloc(sizeof(struct Game_State_Type));

        initGamestate(Gupd_Recive);
      
        memcpy(Gupd_Recive, setup->recv_Pack->data, sizeof(struct Game_State_Type));

        for (int i=0; i < current->nrOfPlayers; i++) {
            if (current->localPlayerNr-1 != i) {
                current->player_Pos_X[i] = Gupd_Recive->player_Pos_X[i];
                current->player_Pos_Y[i] = Gupd_Recive->player_Pos_Y[i];
                current->player_Alive[i] = Gupd_Recive->player_Alive[i];
                current->playerScore[i] = Gupd_Recive->playerScore[i];
            }

            current->pushAngle[i] = Gupd_Recive->pushAngle[i];
            if (Gupd_Recive->pushAngle[i] != 0)
            {
                printf("Angle: %d Player: %d\n", Gupd_Recive->pushAngle[i], i);
            }
        }

        //printf("Reciving: %d %d\n", (int)Gupd_Recive->player_Pos_X[0], (int)Gupd_Recive->player_Pos_Y[0]);

        current->obstacle_bottom = Gupd_Recive->obstacle_bottom;
        current->obstacle_top = Gupd_Recive->obstacle_top;
        current->obstacle_change_flag = Gupd_Recive->obstacle_change_flag;

        for (int i = 0; current->nrOfPlayers > i; i++) {
            if (current->localPlayerNr - 1 != i)
                current->pushAngle[i] = 0;
        }

        
        current->powerUpRect = Gupd_Recive->powerUpRect;
        current->powerUpDir = Gupd_Recive->powerUpDir;
        current->powerUpType = Gupd_Recive->powerUpType;
        current->powerUp_change_flag = Gupd_Recive->powerUp_change_flag;

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

            current->change_flag = 1;

            Game_State_Send Gupd_Recive = malloc(sizeof(struct Game_State_Send_Type));
            memcpy(Gupd_Recive, setup->recv_Pack->data, sizeof(struct Game_State_Send_Type));

            current->connectionTimers[Gupd_Recive->localPlayerNr] = 0;

            current->player_Pos_X[Gupd_Recive->localPlayerNr - 1] = Gupd_Recive->player_Pos_X;
            current->player_Pos_Y[Gupd_Recive->localPlayerNr - 1] = Gupd_Recive->player_Pos_Y;
            current->player_Alive[Gupd_Recive->localPlayerNr - 1] = Gupd_Recive->player_Alive;
            current->playerScore[Gupd_Recive->localPlayerNr - 1] = Gupd_Recive->playerScore;

            for (int i = 0; current->nrOfPlayers > i; i++) {
                if (Gupd_Recive->pushAngle[i] != 0)
                {
                    current->pushAngle[i] = Gupd_Recive->pushAngle[i];
                    printf("Angle: %d Player: %d\n", Gupd_Recive->pushAngle[i],i);
                }
            }


            free(Gupd_Recive);
        }
    }


    //Om flaggan är satt så finns det ny data att skicka
    if (current->change_flag == 1) {

        memcpy(setup->send_Pack->data, current, sizeof(struct Game_State_Type)); //kopierar Game_state till paketet
        setup->send_Pack->len = sizeof(struct Game_State_Type);

        for (int i = 0; current->nrOfPlayers > i; i++) {
            setup->send_Pack->address.host = setup->sendingIP[i].host;
            setup->send_Pack->address.port = setup->sendingIP[i].port;
            SDLNet_UDP_Send(setup->send_Sock, -1, setup->send_Pack);
        }

        current->change_flag = 0;
        current->obstacle_change_flag = 0;
        for (int i = 0; current->nrOfPlayers > i; i++) {
            if(current->localPlayerNr-1!=i)
                current->pushAngle[i] = 0;
        }
        current->powerUp_change_flag = 0;
    }   

    //kollar om det finns ett packet att hämta

    return 0;
}


int serverLobbyConnection(Game_State current)
{

    IPaddress ip1;
    IPaddress ip_Recive;
    char playerName[100] = "NULL";
    char close[] = "Close";
    int port = 2002;
    TCP_Communication communication = malloc(sizeof(struct TCP_Communication_Type));

    initTCPCom(communication);

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

     SDLNet_ResolveHost(&ip1, NULL, port);

     TCPsocket server = SDLNet_TCP_Open(&ip1);
     TCPsocket client;
       
     do {

         client = SDLNet_TCP_Accept(server);


         if (client)
         {
             
                 SDLNet_TCP_Recv(client, communication, sizeof(struct TCP_Communication_Type)); //Tar emot namnet som skickas över strömmen

                     ip_Recive = *SDLNet_TCP_GetPeerAddress(client); //Väntar tills en klient kopplar upp sig och tar IP:n från TCP strömmen
               
                     //current->ipAdressCache = SDLNet_ResolveIP(&ip_Recive); //Test snabbare koppling

                     strncpy(current->ipAdressCache, SDLNet_ResolveIP(&ip_Recive), IP_LENGTH);
                     strncpy(current->playerNames[current->nrOfPlayers],communication->playerName,NAME_LENGTH);
                     current->nrOfPlayers++;
                     current->newPlayerFlag = 1;

             

             for (int i = 0; current->nrOfPlayers-1 > i; i++) {
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
     } while (current->lobbyRunningFlag);


     free(communication);
     SDLNet_TCP_Close(server);
     SDLNet_Quit();

    return 0;
}



int clientLobbyConnection(char playerIp[], char playerName[], Game_State current)
{

    int port = 2002;
    int timer = 0;
    int connectionTime = 3;
    TCPsocket server;
    IPaddress ip1;
    TCP_Communication communication = malloc(sizeof(struct TCP_Communication_Type));
    char sent[10] = "NULL";

    initTCPCom(communication);
    strncpy(communication->playerName, playerName, NAME_LENGTH);

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    SDLNet_ResolveHost(&ip1, playerIp, port);

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

    int port;
    TCPsocket server;
    IPaddress ip1;
    char sent[10] = "NULL";
    port = 2002;

    initTCPCom(communication);

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    SDLNet_ResolveHost(&ip1, playerIp, port);
    do {
        server = SDLNet_TCP_Open(&ip1);
    } while (server == NULL);

    SDLNet_TCP_Send(server, communication, sizeof(struct TCP_Communication_Type));

    free(communication);
    SDLNet_TCP_Close(server);

    return 0;
}

int clientLobbyWait(Game_State current)
{
    IPaddress ip1;
    int port;
    port = 2002;
    TCP_Communication communication = malloc(sizeof(struct TCP_Communication_Type));

    char playerName[100] = "NULL";
    char close[] = "Close";

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    initTCPCom(communication);
    SDLNet_ResolveHost(&ip1, NULL, port);

    TCPsocket server = SDLNet_TCP_Open(&ip1);
    TCPsocket client;

    do {

        client = SDLNet_TCP_Accept(server);

        if (client)
        {
                SDLNet_TCP_Recv(client, communication, sizeof(struct TCP_Communication_Type)); //Tar emot namnet som skickas över strömmen

                if (!communication->startGame) {
                    current->lobbyRunningFlag = 0;
                }
                else{
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
    return current->player_Pos_X[playerNr];
}

int getGameStatePlayerPosY(Game_State current, int playerNr)
{
    return current->player_Pos_Y[playerNr];

}

int SetGameStatePlayerPosX(Game_State current, SDL_Rect* playerPos[])
{
    
    if (current->player_Pos_X[current->localPlayerNr-1] != playerPos[current->localPlayerNr-1]->x) {

        current->change_flag = 1;
        current->player_Pos_X[current->localPlayerNr-1] = playerPos[current->localPlayerNr-1]->x;
    }
    return 0;
}

int SetGameStatePlayerPosY(Game_State current, SDL_Rect* playerPos[])
{

    if (current->player_Pos_Y[current->localPlayerNr-1] != playerPos[current->localPlayerNr-1]->y) {

        current->change_flag = 1;
        current->player_Pos_Y[current->localPlayerNr-1] = playerPos[current->localPlayerNr-1]->y;
    }
    return 0;
}

int SetGameStatePlayerStatus(Game_State current, Player players[])
{
    if (current->player_Alive[current->localPlayerNr-1] != getPlayerStatus(players[current->localPlayerNr-1]) ) {

        current->change_flag = 1;
        current->player_Alive[current->localPlayerNr-1] = getPlayerStatus(players[current->localPlayerNr-1]);
    }

    if (current->playerScore[current->localPlayerNr - 1] != getPlayerScore(players[current->localPlayerNr - 1])) {

        current->change_flag = 1;
        current->playerScore[current->localPlayerNr - 1] = getPlayerScore(players[current->localPlayerNr - 1]);
    }

    return 0;
}

//Uppdaterar obstacle i Game_state
int SetObstacle(Game_State Gupd, Obstacle Send_obstacles)
{
    Gupd->change_flag = 1; //sätter change flaggan så att det skickas över nätverket
    Gupd->obstacle_change_flag = 1; //obstacle flaggan visar för klienten att ett nytt obstacle finns att rendra
    Gupd->obstacle_top = getRectfromObstacle(getNextObsFromList(Send_obstacles), true);
    Gupd->obstacle_bottom = getRectfromObstacle(getNextObsFromList(Send_obstacles), false);
    
    return 0;
}

Obstacle ReciveObstacle(Game_State Gupd)
{
    Obstacle recive_obstacle = createClientObstacle(Gupd->obstacle_top, Gupd->obstacle_bottom);

    Gupd->obstacle_change_flag = 0;

    return recive_obstacle;
}

void SetPowerUp(Game_State current, PowerUp aPowerUp) {
    current->change_flag = 1; //sätter change flaggan så att det skickas över nätverket
    current->powerUp_change_flag = 1; //powerup flaggan visar för klienten att en ny powerup genererats
    current->powerUpRect = getPowerUpRect(aPowerUp); //kopierar över powerups attribut
    current->powerUpDir = getPowerUpDir(aPowerUp); //kopierar över powerups attribut
    current->powerUpType = getPowerUpType(aPowerUp); //kopierar över powerups attribut
}

PowerUp ReceivePowerUp(Game_State current) {
    PowerUp newPowerUp = clientSpawnPowerUp(current->powerUpRect, current->powerUpDir, current->powerUpType);
    current->powerUp_change_flag = 0;
    return newPowerUp;
}

void renderConnections(Game_State current) {

    int max_disconnection_Time = 10;

    for (int i = 0; current->nrOfPlayers - 1 > i; i++) {
        
        if (current->connectionTimers[i] < max_disconnection_Time) {
            printf("Player: %s disconnected\n", current->playerNames[i]);
        }
    }

}

int initTCPCom(TCP_Communication communication) {
    communication->leftGame = 0;
    communication->connectionOpen = 1;
    communication->startGame = 0;
    communication->recived = 0;

    return 0;
}