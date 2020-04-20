#include "Network.h"



//Creates Gamestate for sending over network
int create_Game_state(int Player_position_x, int Player_position_y, Game_State Gupd) {

    Gupd-> Player_position_x = Player_position_x;
    Gupd-> Player_position_y = Player_position_y;
    Gupd-> opponent_position_x = Player_position_x;
    Gupd-> opponent_position_y = Player_position_y;

    Gupd->change_flag = 0;
    Gupd->obstacle_change_flag = 0;
    Gupd->opponent_alive = true;
    Gupd->player_alive = true;


    return 0;
    //Skapar en Game state struct med startvärden och returnerar den
}


// int netowrk and check what port free
int int_network(char playerIp[], UDP_Config setup, int port1, int port2)
{

    printf("%s\n", playerIp);


    //Öppnar en socket för att ta skicka data
    if (!(setup->sd1 = (SDLNet_UDP_Open(0))))
    {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
   
    //Öppnar en port för att ta emot data om port 2000 är upptagen och tar den 2001.
    if (setup->sd2 = (SDLNet_UDP_Open(port1)))
    {

        setup->port = port1;

        if (SDLNet_ResolveHost(&setup->ip, playerIp, port2) == -1) {

            printf("SDLNet_ResolveHost(%s %d): %s\n", playerIp, port2, SDLNet_GetError());
            exit(EXIT_FAILURE);
        }

        
    }else if(setup->sd2 = (SDLNet_UDP_Open(port2)))
    {
        setup->port = port2;

        if (SDLNet_ResolveHost(&setup->ip, playerIp, port1) == -1) {

            printf("SDLNet_ResolveHost(%s %d): %s\n", playerIp, port1, SDLNet_GetError());
            exit(EXIT_FAILURE);
        }
    }

    //Allokerar plats för två paket
    if (!((setup->p1 = SDLNet_AllocPacket(1024)) && (setup->p2 = SDLNet_AllocPacket(1024))))
    {
        printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    return 0;
}


//sends and recives game data over the network
int sendAndRecive(Game_State Gupd, UDP_Config setup, SDL_Rect *local_player, SDL_Rect *local_opponent)
{
    updateGameSending(Gupd, local_player);
    networkCommunication(Gupd, setup);
    updateGameReciving(Gupd, local_opponent);

    return 0;
}


//Updates the Gamestate with information that is going to be sent
int updateGameSending(Game_State Gupd, SDL_Rect* local_player)
{
    SetPlayerPosX(Gupd, local_player->x);
    SetPlayerPosY(Gupd, local_player->y);
    
    return 0;
}

//
int networkCommunication(Game_State Gupd, UDP_Config setup)
{
    Game_State Gupd_Recive = malloc(sizeof(struct Game_State_Type));

    //Om flaggan är satt så finns det ny data att skicka
    if (Gupd->change_flag == 1) {

        //printf("%d %d\n", (int)Gupd->Player_position_x, (int)Gupd->Player_position_y);

        Gupd->change_flag = 0;

        memcpy(setup->p1->data, Gupd, sizeof(struct Game_State_Type)); //kopierar Game_state till paketet
        setup->p1->len = sizeof(struct Game_State_Type);
        setup->p1->address.host = setup->ip.host;	
        setup->p1->address.port = setup->ip.port;	
        SDLNet_UDP_Send(setup->sd1, -1, setup->p1);

        Gupd->obstacle_change_flag = 0;

    }

    //kollar om det finns ett packet att hämta
    if (SDLNet_UDP_Recv(setup->sd2, setup->p2)) {

        memcpy(Gupd_Recive, setup->p2->data, sizeof(struct Game_State_Type));
        Gupd->opponent_position_x = Gupd_Recive->Player_position_x;
        Gupd->opponent_position_y = Gupd_Recive->Player_position_y;
        Gupd->opponent_alive = Gupd_Recive->player_alive;

        Gupd->obstacle_bottom = Gupd_Recive->obstacle_bottom;
        Gupd->obstacle_top = Gupd_Recive->obstacle_top;
        Gupd->obstacle_change_flag = Gupd_Recive->obstacle_change_flag;
        //printf("Reciving Obstacle: %d\n", Gupd->obstacle_bottom.y);

    }

    return 0;

}

//Öppnar en TCP ström och väntar på en klient
int serverConnection(char playerIp[], UDP_Config setup, int sync)
{

    IPaddress ip1;
    IPaddress ip_Recive;
    char text[100];

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if (sync == 1) //kollar om sync flaggan är satt
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
                SDLNet_TCP_Recv(client, text, 100); //Tar emot namnet som skickas över strömmen
                ip_Recive = *SDLNet_TCP_GetPeerAddress(client); //Väntar tills en klient kopplar upp sig och tar IP:n från TCP strömmen
            }

            connection_flag++;
        } while (!client);

        playerIp = SDLNet_ResolveIP(&ip_Recive);

        printf("%s\n", playerIp);
        printf("%s\n", text);

        SDLNet_TCP_Close(client);
    }

    int_network(playerIp, setup, 2001, 2000); //initierar nätverket för servern

    return 0;
}


//Öppnar en ström till servern och skickar namnet, vilket startar spelet. 
int clientConnection(UDP_Config setup,char playerIp[], char playerName[], int sync)
{
    
    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if (sync == 1)
    {
        IPaddress ip1;

        printf("%s\n", playerIp);

        SDLNet_ResolveHost(&ip1, playerIp, 2005);
        TCPsocket client = SDLNet_TCP_Open(&ip1); //Öppnar en TCP ström.
        SDLNet_TCP_Send(client, playerName, strlen(playerName) + 1); //Skickar namnet på spelaren.
        
        SDLNet_TCP_Close(client);
    }

    int_network(playerIp, setup, 2000, 2001); //initierar nätverket för klienten
    
    return 0;
}

//Stänger ner och rensar
int Close_SDLNet(UDP_Config setup)
{
    SDLNet_FreePacket(setup->p1);
    SDLNet_FreePacket(setup->p2);
    SDLNet_Quit();

    return 0;
}

int updateGameReciving(Game_State Gupd, SDL_Rect* Local_opponent)
{
    Local_opponent->x = Gupd->opponent_position_x;
    Local_opponent->y = Gupd->opponent_position_y;

    return 0;
}

int getPlayerPosX(Game_State Gupd)
{
    return Gupd->Player_position_x;

}

int getPlayerPosY(Game_State Gupd)
{
    return Gupd->Player_position_y;

}

int getOpponentPosX(Game_State Gupd)
{
    return Gupd->opponent_position_x;
}

int getOpponentPosY(Game_State Gupd)
{
    return Gupd->opponent_position_y;
}



int SetPlayerPosX(Game_State Gupd, int Player_posX)
{
    
    if (Gupd->Player_position_x != Player_posX) {

        Gupd->change_flag = 1;
        Gupd->Player_position_x = Player_posX;
    }
    return 0;
}

int SetPlayerPosY(Game_State Gupd, int Player_posY)
{
    if (Gupd->Player_position_y != Player_posY) {

        Gupd->change_flag = 1;
        Gupd->Player_position_y = Player_posY;
    }
    return 0;
}

int SetPlayerAlive(Game_State Gupd, bool Alive_Local)
{
    if (Gupd->player_alive != Alive_Local) {

        Gupd->change_flag = 1;
        Gupd->player_alive = Alive_Local;
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