#include "Network.h"

int create_Game_state(int Player_position_x, int Player_position_y, Game_State Gupd) {

    Gupd-> Player_position_x = Player_position_x;
    Gupd-> Player_position_y = Player_position_y;
    Gupd-> opponent_position_x = Player_position_x;
    Gupd-> opponent_position_y = Player_position_y;

    Gupd->change_flag = 0;
    Gupd->obstacle_change_flag = 0;

    return 0;
    //Skapar en Game state struct med startvärden och returnerar den
}

int int_network(char IP_input[IP_LENGHT], int port, UDP_Config setup)
{

    if (SDLNet_Init() < 0)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }


    if (!(setup->sd1 = (SDLNet_UDP_Open(0))))
    {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
   
  
    if (!(setup->sd2 = (SDLNet_UDP_Open(2000))))
    {
        if (!(setup->sd2 = (SDLNet_UDP_Open(2001)))) {
            printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
            exit(EXIT_FAILURE);
        }

        if (SDLNet_ResolveHost(&setup->ip, "127.0.0.1", 2000) == -1) {

            printf("SDLNet_ResolveHost(%s %d): %s\n", IP_input, port, SDLNet_GetError());
            exit(EXIT_FAILURE);
        }

        //kollar om port 2000 är upptagen och tar 2001 om den är det. 
    }else if(SDLNet_ResolveHost(&setup->ip, "127.0.0.1", 2001) == -1)
    {
        printf("SDLNet_ResolveHost(%s %d): %s\n", IP_input, port, SDLNet_GetError());
        exit(EXIT_FAILURE);
    }


    if (!((setup->p1 = SDLNet_AllocPacket(1024)) && (setup->p2 = SDLNet_AllocPacket(1024))))
    {
        printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    setup->port = port;


    return 0;
    //initierar en socket och returnerar den
}


int sendAndRecive(Game_State Gupd, UDP_Config setup, SDL_Rect *local_player, SDL_Rect *local_opponent)
{
    updateGameSending(Gupd, local_player);
    networkCommunication(Gupd, setup);
    updateGameReciving(Gupd, local_opponent);

    return 0;
}

int updateGameSending(Game_State Gupd, SDL_Rect* local_player)
{
    SetPlayerPosX(Gupd, local_player->x);
    SetPlayerPosY(Gupd, local_player->y);
    
    return 0;
}

int networkCommunication(Game_State Gupd, UDP_Config setup)
{
    Game_State Gupd_Recive = malloc(sizeof(struct Game_State_Type));

    if (Gupd->change_flag == 1) {

        //printf("%d %d\n", (int)Gupd->Player_position_x, (int)Gupd->Player_position_y);

        Gupd->change_flag = 0;

        memcpy(setup->p1->data, Gupd, sizeof(struct Game_State_Type));
        setup->p1->len = sizeof(struct Game_State_Type);
        setup->p1->address.host = setup->ip.host;	/* Set the destination host */
        setup->p1->address.port = setup->ip.port;	/* And destination port */
        SDLNet_UDP_Send(setup->sd1, -1, setup->p1);

    }

    if (SDLNet_UDP_Recv(setup->sd2, setup->p2)) {

        memcpy(Gupd_Recive, setup->p2->data, sizeof(struct Game_State_Type));
        Gupd->opponent_position_x = Gupd_Recive->Player_position_x;
        Gupd->opponent_position_y = Gupd_Recive->Player_position_y;
        Gupd->opponent_alive = Gupd_Recive->player_alive;

        Gupd->obstacle_bottom = Gupd_Recive->obstacle_bottom;
        Gupd->obstacle_top = Gupd_Recive->obstacle_top;
       // printf("Reciving Obstacle: %d\n", Gupd->obstacle_bottom.y);

    }

    return 0;

}

int updateGameReciving(Game_State Gupd, SDL_Rect *Local_opponent)
{
    Local_opponent->x = Gupd->opponent_position_x;
    Local_opponent->y = Gupd->opponent_position_y;

    return 0;
}

int Send_Text(char Text[MAX_TEXT_LENGHT], UDP_Config setup)
{

    TCPsocket server = SDLNet_TCP_Open(&setup->ip);
    TCPsocket client;

    do
    {
        client = SDLNet_TCP_Accept(server);
        if (client)
        {
            SDLNet_TCP_Send(client, Text, strlen(Text) + 1);
            SDLNet_TCP_Close(client);
            break;
        }
    } while (!client);
    SDLNet_TCP_Close(server);
    return 0;
}

int Close_SDLNet(UDP_Config setup)
{
    SDLNet_FreePacket(setup->p1);
    SDLNet_FreePacket(setup->p2);
    SDLNet_Quit();

    return 0;
    //Stänger ner och rensar
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

int SetObstacle(Game_State Gupd, Obstacle Send_obstacles)
{
    Gupd->change_flag = 1;
    Gupd->obstacle_change_flag = 1;
    Gupd->obstacle_top = Send_obstacles->next->top;
    Gupd->obstacle_bottom = Send_obstacles->next->bottom;
    
    return 0;
}

Obstacle ReciveObstacle(Game_State Gupd)
{
    Obstacle recive_obstacle = malloc(sizeof(struct obstacle_type));

    recive_obstacle->top = Gupd->obstacle_top;
    recive_obstacle->bottom = Gupd->obstacle_bottom;
    recive_obstacle->next = NULL; 

    Gupd->obstacle_change_flag = 0;

    return recive_obstacle;
}
