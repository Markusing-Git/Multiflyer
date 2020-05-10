#include "Network.h"



//Creates Gamestate for sending over network
int start_Game_state(Player playerList[], Game_State current) {

    for (int i=0; current->nrOfPlayers > i; i++){

        current->player_Pos_X[i] = getPlayerPoint((playerList[i]),'x');
        current->player_Pos_Y[i] = getPlayerPoint((playerList[i]), 'y');
    }
    return 0;
    //Skapar en Game state struct med startv�rden och returnerar den
}

int initGamestate(Game_State current)
{
    for (int i = 0; MAX_PLAYERS > i; i++) {
        current->player_Pos_X[i] = 0;
        current->player_Pos_Y[i] = 0;
        current->player_Alive[i] = true;
        current->pushAngle[i] = 0;
    }
    current->nrOfPlayers = 0;
    current->change_flag = 0;
    current->obstacle_change_flag = 0;
    current->lobbyRunningFlag = 1;
    current->newPlayerFlag = 0;
    current->localPlayerNr = 0;

    return 0;
}


// int netowrk and check what port free
int init_client_network(char playerIp[], UDP_Client_Config setup, Game_State current)
{
    printf("%s\n", playerIp);

    int sendPort = 2000;
    int recvPort = 2050;

    //�ppnar en socket f�r att ta skicka data
    if (!(setup->send_Sock = (SDLNet_UDP_Open(0))))
    {
        
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
   
    //�ppnar en port f�r att ta emot data om port 2000 �r upptagen och tar den 2001.
    if (setup->recv_Sock[0] = (SDLNet_UDP_Open(recvPort + current->localPlayerNr))) //�ppnar en port f�r att kunna 
    {
        setup->port[0] = recvPort + current->localPlayerNr;

        if (SDLNet_ResolveHost(&setup->sendingIP[0], playerIp, sendPort + current->localPlayerNr) == -1) {

            printf("SDLNet_ResolveHost(%s %d): %s\n", playerIp, sendPort + current->localPlayerNr, SDLNet_GetError());
            exit(EXIT_FAILURE);
        }
    }

    //Allokerar plats f�r tv� paket
    if (!((setup->send_Pack = SDLNet_AllocPacket(2048)) && (setup->recv_Pack = SDLNet_AllocPacket(2048))))
    {
        printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    return 0;
}

int init_Server_network(UDP_Client_Config setup, Game_State current)
{

    int sendPort = 2052;
    int recvPort = 2002;

    //�ppnar en socket f�r att ta skicka data
    if (!(setup->send_Sock = (SDLNet_UDP_Open(0))))
    {
        exit(EXIT_FAILURE);
    }

    for (int i = 0; current->nrOfPlayers-1> i; i++)
    {    
        if (setup->recv_Sock[i] = (SDLNet_UDP_Open(recvPort + i))) //�ppnar en port f�r att kunna 
        {
            setup->port[i] = recvPort + current->localPlayerNr;

            if (SDLNet_ResolveHost(&setup->sendingIP[i], setup->playerIp[i], sendPort + i) == -1) {

                printf("SDLNet_ResolveHost(%s %d): %s\n", setup->playerIp[i], sendPort + i, SDLNet_GetError());
                exit(EXIT_FAILURE);
            }
        }

    }

    //Allokerar plats f�r tv� paket
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

    SetPlayerPosX(current, playerPos);
    SetPlayerPosY(current, playerPos);
    SetPlayerAlive(current, players);

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

    //Om flaggan �r satt s� finns det ny data att skicka
    if (current->change_flag == 1) {

        Game_State_Send Gupd_Sending = malloc(sizeof(struct Game_State_Send_Type));

        Gupd_Sending->player_Pos_X = current->player_Pos_X[current->localPlayerNr - 1];
        Gupd_Sending->player_Pos_Y = current->player_Pos_Y[current->localPlayerNr - 1];
        Gupd_Sending->player_Alive = current->player_Alive[current->localPlayerNr - 1];
        

        //printf("Client Sending: %d\n", Gupd_Sending->player_Pos_Y);

        memcpy(setup->send_Pack->data, Gupd_Sending, sizeof(struct Game_State_Send_Type)); //kopierar Game_state till paketet
        setup->send_Pack->len = sizeof(struct Game_State_Send_Type);
        setup->send_Pack->address.host = setup->sendingIP[0].host;	
        setup->send_Pack->address.port = setup->sendingIP[0].port;	
        SDLNet_UDP_Send(setup->send_Sock, -1, setup->send_Pack);

        current->change_flag = 0;
        current->obstacle_change_flag = 0;

        free(Gupd_Sending);
    }

    //kollar om det finns ett packet att h�mta
    if (SDLNet_UDP_Recv(setup->recv_Sock[0], setup->recv_Pack)) {

        Game_State Gupd_Recive = malloc(sizeof(struct Game_State_Type));

        initGamestate(Gupd_Recive);
      
        memcpy(Gupd_Recive, setup->recv_Pack->data, sizeof(struct Game_State_Type));

        for (int i=0; i < current->nrOfPlayers; i++) {
            if (current->localPlayerNr-1 != i) {
                current->player_Pos_X[i] = Gupd_Recive->player_Pos_X[i];
                current->player_Pos_Y[i] = Gupd_Recive->player_Pos_Y[i];
                current->player_Alive[i] = Gupd_Recive->player_Alive[i];
            }
        }

        //printf("Reciving: %d %d\n", (int)Gupd_Recive->player_Pos_X[0], (int)Gupd_Recive->player_Pos_Y[0]);

        current->obstacle_bottom = Gupd_Recive->obstacle_bottom;
        current->obstacle_top = Gupd_Recive->obstacle_top;
        current->obstacle_change_flag = Gupd_Recive->obstacle_change_flag;

        free(Gupd_Recive);
    }

    return 0;
}


int networkCommunicationServer(Game_State current, UDP_Client_Config setup)
{

    //Om flaggan �r satt s� finns det ny data att skicka
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
    }   

    //kollar om det finns ett packet att h�mta

    for (int i = 0; current->nrOfPlayers - 1>i; i++)
    {
        if (SDLNet_UDP_Recv(setup->recv_Sock[i], setup->recv_Pack)) {

            current->change_flag = 1;

            Game_State_Send Gupd_Recive = malloc(sizeof(struct Game_State_Send_Type));

            //initGamestate(Gupd_Recive);

            memcpy(Gupd_Recive, setup->recv_Pack->data, sizeof(struct Game_State_Send_Type));

            current->player_Pos_X[i+1] = Gupd_Recive->player_Pos_X;
            current->player_Pos_Y[i+1] = Gupd_Recive->player_Pos_Y;
            current->player_Alive[i+1] = Gupd_Recive->player_Alive;


            free(Gupd_Recive);
        }
    }
    

    return 0;
}


int serverLobbyConnection(Game_State current)
{

    IPaddress ip1;
    IPaddress ip_Recive;
    char playerName[100] = "NULL";
    char close[] = "Close";

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }


     SDLNet_ResolveHost(&ip1, NULL, 2005);

     TCPsocket server = SDLNet_TCP_Open(&ip1);
     TCPsocket client;
         
     do {

         client = SDLNet_TCP_Accept(server);


         if (client)
         {
             
             do {
                 SDLNet_TCP_Recv(client, playerName, 100); //Tar emot namnet som skickas �ver str�mmen
                 printf("%s", playerName);
                 if (playerName != "NULL") {
                     ip_Recive = *SDLNet_TCP_GetPeerAddress(client); //V�ntar tills en klient kopplar upp sig och tar IP:n fr�n TCP str�mmen
                     strncpy(current->ipAdressCache, SDLNet_ResolveIP(&ip_Recive), IP_LENGTH);
                     strcpy(current->playerNames[current->nrOfPlayers],playerName);
                     current->nrOfPlayers++;
                     current->newPlayerFlag = 1;
                 }
             } while (!strcmp(playerName, "NULL"));

             

             for (int i = 0; current->nrOfPlayers-1 > i; i++) {
                 SDLNet_TCP_Send(client, current->playerNames[i], strlen(current->playerNames[i])+1);
                 SDL_Delay(150);
             }

             SDLNet_TCP_Send(client, close, strlen(close) + 1);

             SDL_Delay(150);
         }

     } while (current->lobbyRunningFlag);


     SDLNet_TCP_Close(server);
     SDLNet_Quit();

    return 0;
}



int clientLobbyConnection(char playerIp[], char playerName[], Game_State current)
{

    int port = 2005;
    TCPsocket server;
    IPaddress ip1;
    char sent[10] = "NULL";


    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }


    SDLNet_ResolveHost(&ip1, playerIp, port);



    do {
        server = SDLNet_TCP_Open(&ip1);
    } while (server == NULL);

    SDLNet_TCP_Send(server, playerName, strlen(playerName) + 1);

    do {    
            SDLNet_TCP_Recv(server, sent, NAME_LENGTH);

            if ((strcmp(sent, "Close") != 0)) {
                SDLNet_TCP_Send(server, playerName, strlen(playerName) + 1); //Skickar namnet p� spelaren.
            }
            

            if ((strcmp(sent, "NULL")!=0) && (strcmp(sent, "Close") != 0)) {
                
                strcpy(current->playerNames[current->nrOfPlayers], sent);
                current->nrOfPlayers++;
                current->localPlayerNr++;
                strcpy(sent, "NULL");
            }
            
    } while ((strcmp(sent, "Close")!=0));


    strcpy(current->playerNames[current->nrOfPlayers], playerName);
    current->nrOfPlayers++;
    current->localPlayerNr++;

    SDLNet_TCP_Close(server);

    return 0;
}

int serverSendPlayer(char playerIp[], char playerName[],int localPlayerNr, Game_State current)
{

    int port = 2006+localPlayerNr;
    TCPsocket server;
    IPaddress ip1;
    char sent[10] = "NULL";


    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    SDLNet_ResolveHost(&ip1,playerIp, port);

    do {
        server = SDLNet_TCP_Open(&ip1);
    } while (server == NULL);

    SDLNet_TCP_Send(server, playerName, strlen(playerName) + 1);

    do {
        SDLNet_TCP_Recv(server, sent, NAME_LENGTH);
    } while ((strcmp(sent, "Close") != 0));

    SDLNet_TCP_Close(server);

    return 0;
}

int clientLobbyWait(Game_State current)
{
    IPaddress ip1;
    int port = 2006 + current->localPlayerNr;
    char playerName[100] = "NULL";
    char close[] = "Close";

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
            do {
                SDLNet_TCP_Recv(client, playerName, 100); //Tar emot namnet som skickas �ver str�mmen
                if (playerName != "NULL") {
                    strcpy(current->playerNames[current->nrOfPlayers], playerName);
                    current->nrOfPlayers++;
                    current->newPlayerFlag = 1;
                }
            } while (!strcmp(playerName, "NULL"));

            SDLNet_TCP_Send(client, close, strlen(close) + 1);
            SDL_Delay(100);
        }

    } while (current->lobbyRunningFlag);

    SDLNet_TCP_Close(server);
    SDLNet_Quit();

    return 0; 
}

int clientStartGame(Game_State current)
{
    IPaddress ip1;
    int port = 2100 + current->localPlayerNr;


    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    SDLNet_ResolveHost(&ip1, NULL, port);

    TCPsocket server = SDLNet_TCP_Open(&ip1);
    TCPsocket client;


    do
    {
        client = SDLNet_TCP_Accept(server);
        if (client){
            current->lobbyRunningFlag = 0;
        }
    } while (!client);

        SDLNet_TCP_Close(client);

    return 0;
}

int serverStartGame(UDP_Client_Config setup, Game_State current)
{

    TCPsocket client;
    IPaddress ip1;
    int port = 0;

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    for (int i = 0; current->nrOfPlayers - 1> i; i++) {

        port = 2100 + i +2;
        SDLNet_ResolveHost(&ip1, setup->playerIp[i] , port);

        client = SDLNet_TCP_Open(&ip1);
        //SDL_Delay(150);

        SDLNet_TCP_Close(client);
    } 
    return 0;
}


//St�nger ner och rensar
int Close_SDLNet(UDP_Client_Config setup, Game_State current)
{
    SDLNet_FreePacket(setup->send_Pack);
    SDLNet_FreePacket(setup->recv_Pack);
    free(setup);
    free(current);
    SDLNet_Quit();

    return 0;
}

int getPlayerPosX(Game_State current, int playerNr)
{
    return current->player_Pos_X[playerNr];
}

int getPlayerPosY(Game_State current, int playerNr)
{
    return current->player_Pos_Y[playerNr];

}

int SetPlayerPosX(Game_State current, SDL_Rect* playerPos[])
{
    
    if (current->player_Pos_X[current->localPlayerNr-1] != playerPos[current->localPlayerNr-1]->x) {

        current->change_flag = 1;
        current->player_Pos_X[current->localPlayerNr-1] = playerPos[current->localPlayerNr-1]->x;
    }
    return 0;
}

int SetPlayerPosY(Game_State current, SDL_Rect* playerPos[])
{

    if (current->player_Pos_Y[current->localPlayerNr-1] != playerPos[current->localPlayerNr-1]->y) {

        current->change_flag = 1;
        current->player_Pos_Y[current->localPlayerNr-1] = playerPos[current->localPlayerNr-1]->y;
    }
    return 0;
}

int SetPlayerAlive(Game_State current, Player players[])
{
    if (current->player_Alive[current->localPlayerNr-1] != getPlayerStatus(players[current->localPlayerNr-1]) ) {

        current->change_flag = 1;
        current->player_Alive[current->localPlayerNr-1] = getPlayerStatus(players[current->localPlayerNr-1]);
    }
    return 0;
}

//Uppdaterar obstacle i Game_state
int SetObstacle(Game_State Gupd, Obstacle Send_obstacles)
{
    Gupd->change_flag = 1; //s�tter change flaggan s� att det skickas �ver n�tverket
    Gupd->obstacle_change_flag = 1; //obstacle flaggan visar f�r klienten att ett nytt obstacle finns att rendra
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