#include "Network.h"



//Creates Gamestate for sending over network
int create_Game_state(Player playerList[], Game_State current, int playerCount) {

    initGamestate(current);

    for (int i=0; playerCount > i; i++){

        current->player_Pos_X[i] = getPlayerPoint((playerList[i]),'x');
        current->player_Pos_Y[i] = getPlayerPoint((playerList[i]), 'y');
        current->player_Alive[i] = true;
    }
    current->nrOfPlayers = playerCount;
    current->change_flag = 0;
    current->obstacle_change_flag = 0;


    return 0;
    //Skapar en Game state struct med startv�rden och returnerar den
}

int initGamestate(Game_State current)
{
    for (int i = 0; MAX_PLAYERS > i; i++) {
        current->player_Pos_X[i] = 0;
        current->player_Pos_Y[i] = 0;
        current->player_Alive[i] = true;
    }
    current->nrOfPlayers = 0;
    current->change_flag = 0;
    current->obstacle_change_flag = 0;

    return 0;
}


// int netowrk and check what port free
int int_client_network(char playerIp[], UDP_Client_Config setup, int port1, int port2)
{

    printf("%s\n", playerIp);


    //�ppnar en socket f�r att ta skicka data
    if (!(setup->send_Sock = (SDLNet_UDP_Open(0))))
    {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
   
    //�ppnar en port f�r att ta emot data om port 2000 �r upptagen och tar den 2001.
    if (setup->rec_Sock = (SDLNet_UDP_Open(port1))) //�ppnar en port f�r att kunna 
    {



        setup->port = port1;

        if (SDLNet_ResolveHost(&setup->server_Ip, playerIp, port2) == -1) {

            printf("SDLNet_ResolveHost(%s %d): %s\n", playerIp, port2, SDLNet_GetError());
            exit(EXIT_FAILURE);
        }

        
    }else if(setup->rec_Sock = (SDLNet_UDP_Open(port2)))
    {
        setup->port = port2;

        if (SDLNet_ResolveHost(&setup->server_Ip, playerIp, port1) == -1) {

            printf("SDLNet_ResolveHost(%s %d): %s\n", playerIp, port1, SDLNet_GetError());
            exit(EXIT_FAILURE);
        }
    }

    //Allokerar plats f�r tv� paket
    if (!((setup->send_Pack = SDLNet_AllocPacket(2048)) && (setup->rec_Pack = SDLNet_AllocPacket(2048))))
    {
        printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    return 0;
}


//sends and recives game data over the network
int sendAndRecive(Game_State Gupd, UDP_Client_Config setup, SDL_Rect *local_player, SDL_Rect *local_opponent)
{
    updateGameSending(Gupd, local_player,0);
    networkCommunicationClient(Gupd, setup);
    updateGameReciving(Gupd, local_opponent,1);

    return 0;
}

//
int networkCommunicationClient(Game_State current, UDP_Client_Config setup)
{

    //Om flaggan �r satt s� finns det ny data att skicka
    if (current->change_flag == 1) {

        //printf("Sending: %d %d\n", (int)current->player_Pos_X[0], (int)current->player_Pos_X[0]);

        current->change_flag = 0;

        memcpy(setup->send_Pack->data, current, sizeof(struct Game_State_Type)); //kopierar Game_state till paketet
        setup->send_Pack->len = sizeof(struct Game_State_Type);
        setup->send_Pack->address.host = setup->server_Ip.host;	
        setup->send_Pack->address.port = setup->server_Ip.port;	
        SDLNet_UDP_Send(setup->send_Sock, -1, setup->send_Pack);
    }

    //kollar om det finns ett packet att h�mta
    if (SDLNet_UDP_Recv(setup->rec_Sock, setup->rec_Pack)) {

        Game_State Gupd_Recive = malloc(sizeof(struct Game_State_Type));

        initGamestate(Gupd_Recive);
       

        memcpy(Gupd_Recive, setup->rec_Pack->data, sizeof(struct Game_State_Type));

        for (int i=0; i < current->nrOfPlayers - 1; i++) {
            current->player_Pos_X[i+1] = Gupd_Recive->player_Pos_X[i];
            current->player_Pos_Y[i+1] = Gupd_Recive->player_Pos_Y[i];
            current->player_Alive[i+1] = Gupd_Recive->player_Alive[i];
        }

        //printf("Reciving: %d %d\n", (int)Gupd_Recive->player_Pos_X[0], (int)Gupd_Recive->player_Pos_Y[0]);

        current->obstacle_bottom = Gupd_Recive->obstacle_bottom;
        current->obstacle_top = Gupd_Recive->obstacle_top;
        current->obstacle_change_flag = Gupd_Recive->obstacle_change_flag;

        if (Gupd_Recive->obstacle_change_flag) {
            printf("Reciving Obstacle: %d\n", current->obstacle_bottom.y);
        }

        free(Gupd_Recive);
    }

    return 0;
}

//�ppnar en TCP str�m och v�ntar p� en klient
int serverConnection(char playerIp[], UDP_Client_Config setup, int sync)
{

    IPaddress ip1;
    IPaddress ip_Recive;
    char text[100];

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if (sync == 1) //kollar om sync flaggan �r satt
    {
        SDLNet_ResolveHost(&ip1, NULL, 2005);

        TCPsocket server = SDLNet_TCP_Open(&ip1);
        TCPsocket client;

        int connection_flag = 1;

        do
        {
            client = SDLNet_TCP_Accept(server);
            if (client)
            {
                printf("Recived\n");
                SDLNet_TCP_Recv(client, text, 100); //Tar emot namnet som skickas �ver str�mmen
                ip_Recive = *SDLNet_TCP_GetPeerAddress(client); //V�ntar tills en klient kopplar upp sig och tar IP:n fr�n TCP str�mmen
            }

            connection_flag++;
        } while (!client);

        playerIp = SDLNet_ResolveIP(&ip_Recive);

        printf("%s\n", playerIp);
        printf("%s\n", text);

        SDLNet_TCP_Close(client);
    }

    //int_network(playerIp, setup, 2001, 2000); //initierar n�tverket f�r servern
    return 0;
}

int serverLobbyConnection(char playerIp[], Game_State current, Lobby hostLobby)
{

    IPaddress ip1;
    IPaddress ip_Recive;
    char playerName[100];

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }


     SDLNet_ResolveHost(&ip1, NULL, 2005);

     TCPsocket server = SDLNet_TCP_Open(&ip1);
     TCPsocket client;
         
     while (1)
     {

         client = SDLNet_TCP_Accept(server);

         if (client)
         {
             printf("Recived\n");
             SDLNet_TCP_Recv(client, playerName, 100); //Tar emot namnet som skickas �ver str�mmen
             ip_Recive = *SDLNet_TCP_GetPeerAddress(client); //V�ntar tills en klient kopplar upp sig och tar IP:n fr�n TCP str�mmen
             //inputNameLobby(hostLobby, current, playerName);
             playerIp = SDLNet_ResolveIP(&ip_Recive);
         }
     }
     SDLNet_TCP_Close(server);


    //int_network(playerIp, setup, 2001, 2000); //initierar n�tverket f�r servern
    return 0;
}

int clientLobbyConnection(char playerIp[], char playerName[])
{

    int port = 2005;
    TCPsocket client;
    TCPsocket server;
    IPaddress ip1;

    printf("init Network\n");

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }



    printf("%s\n", playerIp);

    SDLNet_ResolveHost(&ip1, playerIp, port);


    printf("%s\n", playerName);
    client = SDLNet_TCP_Open(&ip1);

    do {
            SDLNet_TCP_Send(client, playerName, strlen(playerName) + 1); //Skickar namnet p� spelaren.
            //printf("Sending\n");
 
    } while (1);

    //SDLNet_TCP_Close(client);
//int_client_network(serverIp, setup, 2000, 2001); //initierar n�tverket f�r klienten

    return 0;
}

//�ppnar en str�m till servern och skickar namnet, vilket startar spelet. 
int clientConnection(UDP_Client_Config setup,char serverIp[], char playerName[], int sync)
{

    int port = 2005;
    TCPsocket client, server;

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if (sync == 1)
    {
        IPaddress ip1;

        printf("%s\n", serverIp);

        SDLNet_ResolveHost(&ip1, serverIp, port);

        
        client = SDLNet_TCP_Open(&ip1); //�ppnar en TCP str�m.

        do
        {
            server = SDLNet_TCP_Accept(client);
            printf("connection\n");
            if (server)
            {
                SDLNet_TCP_Send(client, playerName, strlen(playerName) + 1); //Skickar namnet p� spelaren.
            }
        } while (!server);

        SDLNet_TCP_Close(client);
    }

    //int_client_network(serverIp, setup, 2000, 2001); //initierar n�tverket f�r klienten
    
    return 0;
}



//St�nger ner och rensar
int Close_SDLNet(UDP_Client_Config setup, Game_State current)
{
    SDLNet_FreePacket(setup->send_Pack);
    SDLNet_FreePacket(setup->rec_Pack);
    free(setup);
    free(current);
    SDLNet_Quit();

    return 0;
}

//Updates the Gamestate with information that is going to be sent
int updateGameSending(Game_State current, SDL_Rect* local_player, int playerNr)
{
    SetPlayerPosX(current, local_player->x,0);
    SetPlayerPosY(current, local_player->y,0);

    return 0;
}

int updateGameReciving(Game_State current, SDL_Rect* Local_player, int playerNr)
{
    Local_player->x = current->player_Pos_X[playerNr];
    Local_player->y = current->player_Pos_Y[playerNr];

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

int SetPlayerPosX(Game_State current, int Player_posX, int playerNr)
{
    
    if (current->player_Pos_X[playerNr] != Player_posX) {

        current->change_flag = 1;
        current->player_Pos_X[playerNr] = Player_posX;
    }
    return 0;
}

int SetPlayerPosY(Game_State current, int Player_posY, int playerNr)
{
    if (current->player_Pos_Y[playerNr] != Player_posY) {

        current->change_flag = 1;
        current->player_Pos_Y[playerNr] = Player_posY;
    }
    return 0;
}

int SetPlayerAlive(Game_State current, bool Alive_Local, int playerNr)
{
    if (current->player_Alive[playerNr] != Alive_Local) {

        current->change_flag = 1;
        current->player_Alive[playerNr] = Alive_Local;
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