#include "Network.h"

void set_Game_state(Player playerList[], Game_State current) {

    for (int i=0; current->nrOfPlayers > i; i++){

        current->playerPosX[i] = getPlayerPoint((playerList[i]),'x');
        current->playerPosY[i] = getPlayerPoint((playerList[i]), 'y');
    }
    current->serverConnection = 0;
}

void initGamestate(Game_State current)
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
}


void initTCPCom(TCP_Communication communication) {
    communication->leftGame = 0;
    communication->connectionOpen = 1;
    communication->startGame = 0;
    communication->recived = 0;
    communication->localPlayerNr = 0;
    communication->serverDisconnect = 0;
}



void init_client_network(char playerIp[], Network_Config setup, Game_State current)
{

    if (!(setup->send_Sock = (SDLNet_UDP_Open(0))))
    {
        
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    printf("%s\n", playerIp);
 

    if (setup->recv_Sock[0] = (SDLNet_UDP_Open(PORT)))
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

    if (!((setup->send_Pack = SDLNet_AllocPacket(sizeof(struct Game_State_Send_Client_Type))) && (setup->recv_Pack = SDLNet_AllocPacket(sizeof(struct Game_State_Send_Server_Type)))))
    {
        printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

void init_Server_network(Network_Config setup, Game_State current)
{

    if (!(setup->send_Sock = (SDLNet_UDP_Open(0))))
    {
        exit(EXIT_FAILURE);
    }

    if (!(setup->recv_Sock[0] = SDLNet_UDP_Open(PORT)))
    {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
 
    for (int i = 0; current->nrOfPlayers-1> i; i++)
    {    

            if (SDLNet_ResolveHost(&setup->sendingIP[i], setup->playerIp[i], PORT) == -1) {

                printf("SDLNet_ResolveHost(%s %d): %s\n", setup->playerIp[i], PORT, SDLNet_GetError());
                exit(EXIT_FAILURE);
            }

    }
    if (!((setup->send_Pack = SDLNet_AllocPacket(sizeof(struct Game_State_Send_Server_Type))) && (setup->recv_Pack = SDLNet_AllocPacket(sizeof(struct Game_State_Send_Client_Type)))))
    {
        printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

void resetClientSDLNet(Network_Config setup)
{
    SDLNet_FreePacket(setup->send_Pack);
    SDLNet_FreePacket(setup->recv_Pack);
    SDLNet_UDP_Close(setup->recv_Sock[0]);
}

void resetServerSDLNet(Network_Config setup, Game_State current)
{
    SDLNet_FreePacket(setup->send_Pack);
    SDLNet_FreePacket(setup->recv_Pack);

    for (int i = 0; current->nrOfPlayers - 1 > i; i++)
        SDLNet_UDP_Close(setup->recv_Sock[0]);
}

void Close_SDLNet(Network_Config setup, Game_State current)
{
    SDLNet_FreePacket(setup->send_Pack);
    SDLNet_FreePacket(setup->recv_Pack);
    free(setup);
    free(current);
    SDLNet_Quit();
}

void sendAndReciveClient(Game_State Gupd, Network_Config setup, SDL_Rect *playerPos[], Player players[])
{
    updateGameSending(Gupd, playerPos,players); //Updates Game_State with local player info
    networkCommunicationClient(Gupd, setup); //Sends Game_State over the network
    updateGameReciving(Gupd, playerPos,players); //Updates Player structures with Remote player info from Game_State
}

void sendAndReciveServer(Game_State Gupd, Network_Config setup, SDL_Rect* playerPos[], Player players[])
{
    updateGameSending(Gupd, playerPos, players);
    networkCommunicationServer(Gupd, setup);
    updateGameReciving(Gupd, playerPos, players);
}


void updateGameSending(Game_State current, SDL_Rect* playerPos[], Player players[])
{

    SetGameStatePlayerPosX(current, playerPos);
    SetGameStatePlayerPosY(current, playerPos);
    SetGameStatePlayerStatus(current, players);

}

void updateGameReciving(Game_State current, SDL_Rect* playerPos[], Player players[])
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
}

//
void networkCommunicationClient(Game_State current, Network_Config setup)
{

    //checks if there is new information to send
    if (current->changeFlag == 1) {

        Game_State_Send_Client Gupd_Sending = malloc(sizeof(struct Game_State_Send_Client_Type));

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

        memcpy(setup->send_Pack->data, Gupd_Sending, sizeof(struct Game_State_Send_Client_Type));
        setup->send_Pack->len = sizeof(struct Game_State_Send_Client_Type);
        setup->send_Pack->address.host = setup->sendingIP[0].host;	
        setup->send_Pack->address.port = setup->sendingIP[0].port;	
        SDLNet_UDP_Send(setup->send_Sock, -1, setup->send_Pack);

        current->changeFlag = 0;
        current->obstacleChangeFlag = 0;
        current->powerUpChangeFlag = 0;

        free(Gupd_Sending);
    }

    current->connectionTimers[0]++; //Adds to the server ping.

    //Checks if there is a packet to recive
    if (SDLNet_UDP_Recv(setup->recv_Sock[0], setup->recv_Pack)) {

        current->connectionTimers[0] = 0; //Resets the server ping. 

        Game_State_Send_Server Gupd_Recive = malloc(sizeof(struct Game_State_Send_Server_Type));
      
        memcpy(Gupd_Recive, setup->recv_Pack->data, sizeof(struct Game_State_Send_Server_Type));

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
        }

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
}


void networkCommunicationServer(Game_State current, Network_Config setup)
{
    for (int i = 1; current->nrOfPlayers> i; i++) {
        current->connectionTimers[i]++; //Adds to clients ping
    }


    for (int i = 0; current->nrOfPlayers - 1 > i; i++)
    {

        if (SDLNet_UDP_Recv(setup->recv_Sock[0], setup->recv_Pack)) {

            current->changeFlag = 1;

            Game_State_Send_Client Gupd_Recive = malloc(sizeof(struct Game_State_Send_Client_Type));
            memcpy(Gupd_Recive, setup->recv_Pack->data, sizeof(struct Game_State_Send_Client_Type));

            current->connectionTimers[Gupd_Recive->localPlayerNr - 1] = 0; //Resets clients ping if package is recived
            
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
                }
            }

            free(Gupd_Recive);
        }
    }

    if (current->changeFlag == 1) {

        Game_State_Send_Server Gupd_Send = malloc(sizeof(struct Game_State_Send_Server_Type));

        for (int i = 0; current->nrOfPlayers > i; i++) {

            Gupd_Send->playerPosX[i] = current->playerPosX[i];
            Gupd_Send->playerPosY[i] = current->playerPosY[i];
            Gupd_Send->playerAlive[i] = current->playerAlive[i];
            Gupd_Send->pushAngle[i] = current->pushAngle[i];

            Gupd_Send->playerScore[i] = current->playerScore[i];

            Gupd_Send->resurected[i] = current->resurected[i];
            Gupd_Send->playerPower[i] = current->playerPower[i];
            Gupd_Send->playerSkin[i] = current->playerSkin[i];
            Gupd_Send->attack[i] = current->attack[i];
        }

        Gupd_Send->obstacleChangeFlag = current->obstacleChangeFlag;
        Gupd_Send->powerUpChangeFlag = current->powerUpChangeFlag;
        Gupd_Send->obstacle_top = current->obstacle_top;
        Gupd_Send->obstacle_bottom = current->obstacle_bottom;
        Gupd_Send->powerUpRect = current->powerUpRect;
        Gupd_Send->powerUpDir = current->powerUpDir;
        Gupd_Send->powerUpType = current->powerUpType;
        Gupd_Send->attackPower = current->attackPower;

        memcpy(setup->send_Pack->data, Gupd_Send, sizeof(struct Game_State_Send_Server_Type));
        setup->send_Pack->len = sizeof(struct Game_State_Send_Server_Type);

        for (int i = 0; current->nrOfPlayers-1 > i; i++) {
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
             
            SDLNet_TCP_Recv(client, communication, sizeof(struct TCP_Communication_Type));

            if (communication->leftGame) {
                current->disconnectionCache = communication->localPlayerNr;
                communication->leftGame = 0;
            }
            else {

                ipRecive = SDLNet_TCP_GetPeerAddress(client); //Gets IP adress from TCP stream

                strncpy(current->ipAdressCache, SDLNet_ResolveIP(ipRecive), IP_LENGTH); //Saves IP to Game_State.
                strncpy(current->playerNames[current->nrOfPlayers], communication->playerName, NAME_LENGTH); //Saves Players name to Game_State
                current->nrOfPlayers++;
                current->newPlayerFlag = 1;



                for (int i = 0; current->nrOfPlayers - 1 > i; i++) {
                    strncpy(communication->playerName, current->playerNames[i], NAME_LENGTH);
                    if (current->nrOfPlayers - 2 == i) {
                        communication->connectionOpen = 0; //If all namnes of players are sent to client tell client it's the last name 
                    }
                    communication->recived = 1;
                    SDLNet_TCP_Send(client, communication, sizeof(struct TCP_Communication_Type));
                    SDL_Delay(150);
                }
            }
         }
     } while (current->lobbyRunningFlag); //Check if there is a new player while the lobby is running


     free(communication);
     SDLNet_TCP_Close(server);
     SDLNet_Quit();

     return 0;
}



int clientLobbyConnection(char playerIp[], char playerName[], Game_State current)
{
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

    server = SDLNet_TCP_Open(&ip1);
   
    if (server==NULL) {
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

void sendToClient(TCP_Communication communication, char playerIp[], Game_State current)
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

void disconnectFromServer(char playerIp[], Game_State current)
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
}

void closeServer(Game_State current, TCP_Communication communication, Network_Config setup)
{
    communication->serverDisconnect = 1;
    for (int i = 0; current->nrOfPlayers - 1 > i; i++) {
        sendToClient(communication, setup->playerIp[i], current);
    }
    current->lobbyRunningFlag = 0;
    current->nrOfPlayers = 0;

    free(communication);
}

void limitPingServer(Game_State current) {

    for (int i = 1; current->nrOfPlayers > i; i++) {

        if (current->connectionTimers[i] > DISCONNECT_TIMER && current->playerAlive[i] != false) {
            current->playerAlive[i] = 0;
            printf("Player: %s disconnected\n", current->playerNames[i]);
        }
    }

}

int limitPingClient(Game_State current) {

    if (current->connectionTimers[0] > DISCONNECT_TIMER) {
        printf("Player(Host): %s disconnected\n", current->playerNames[0]);
        return 1;
    }

    return 0;
}

void removePlayerFromLobby(Game_State current, Network_Config setup, int localPlayerNr) {

    if (current->nrOfPlayers > 2 && localPlayerNr != current->nrOfPlayers) {
        for (int i = localPlayerNr - 1; current->nrOfPlayers > i; i++) {
            strncpy(current->playerNames[i], current->playerNames[i + 1], NAME_LENGTH);
            if (setup != NULL && current->nrOfPlayers - 1 > i) {
                strncpy(setup->playerIp[i - 1], setup->playerIp[i], IP_LENGTH);
            }

        }
    }

    strcpy(current->playerNames[current->nrOfPlayers - 1], "*empty*");
    current->nrOfPlayers = current->nrOfPlayers - 1;
}

void SetGameStatePlayerPosX(Game_State current, SDL_Rect* playerPos[])
{
    
    if (current->playerPosX[current->localPlayerNr-1] != playerPos[current->localPlayerNr-1]->x) {

        current->changeFlag = 1;
        current->playerPosX[current->localPlayerNr-1] = playerPos[current->localPlayerNr-1]->x;
    }
}

void SetGameStatePlayerPosY(Game_State current, SDL_Rect* playerPos[])
{

    if (current->playerPosY[current->localPlayerNr-1] != playerPos[current->localPlayerNr-1]->y) {

        current->changeFlag = 1;
        current->playerPosY[current->localPlayerNr-1] = playerPos[current->localPlayerNr-1]->y;
    }
}

void SetGameStatePlayerStatus(Game_State current, Player players[])
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
}

void SetObstacle(Game_State Gupd, Obstacle Send_obstacles)
{
    Gupd->changeFlag = 1; 
    Gupd->obstacleChangeFlag = 1;
    Gupd->obstacle_top = getRectfromObstacle(getNextObsFromList(Send_obstacles), true);
    Gupd->obstacle_bottom = getRectfromObstacle(getNextObsFromList(Send_obstacles), false);
}

void SetPowerUp(Game_State current, PowerUp aPowerUp) {
    current->changeFlag = 1; 
    current->powerUpChangeFlag = 1;
    current->powerUpRect = getPowerUpRect(aPowerUp); 
    current->powerUpDir = getPowerUpDir(aPowerUp); 
    current->powerUpType = getPowerUpType(aPowerUp);
}

Obstacle ReciveObstacle(Game_State Gupd)
{
    Obstacle recive_obstacle = createClientObstacle(Gupd->obstacle_top, Gupd->obstacle_bottom);

    Gupd->obstacleChangeFlag = 0;

    return recive_obstacle;
}

PowerUp ReceivePowerUp(Game_State current) {
    PowerUp newPowerUp = clientSpawnPowerUp(current->powerUpRect, current->powerUpDir, current->powerUpType);
    current->powerUpChangeFlag = 0;
    return newPowerUp;
}