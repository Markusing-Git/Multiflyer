#include "Network.h"

int create_Game_state(int Player_position_x, int Player_position_y, Game_State Gupd) {

    if (Gupd == NULL) {
        printf("Update Error: Update could not be allocated.\n");
    }
    Gupd-> Player_position_x = Player_position_x;
    Gupd-> Player_position_y = Player_position_y;

    Gupd->change_flag = 0;

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
        setup->sd2 = (SDLNet_UDP_Open(2001));
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        SDLNet_ResolveHost(&setup->ip, "127.0.0.1", 2000);
        //exit(EXIT_FAILURE);
    }else if(SDLNet_ResolveHost(&setup->ip, "127.0.0.1", 2001) == -1)
    {
        printf("SDLNet_ResolveHost(%s %d): %s\n", IP_input, port, SDLNet_GetError());
        exit(EXIT_FAILURE);
    }


    if (!((setup->p1 = SDLNet_AllocPacket(512)) && (setup->p2 = SDLNet_AllocPacket(512))))
    {
        printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }


    setup->port = port;
    setup->port_Set_Flag = 0;


    return 0;
    //initierar en socket och returnerar den


}



int sendAndRecive(Game_State Gupd, UDP_Config setup)
{
    


    if (Gupd-> change_flag == 1) {

        sprintf_s((char*)setup->p1->data,sizeof(Gupd->Player_position_x)+ sizeof(Gupd->Player_position_y) + 2, "%d %d\n", (int)Gupd->Player_position_x, (int)Gupd->Player_position_y);
        setup->p1->address.host = setup->ip.host;	/* Set the destination host */
        setup->p1->address.port = setup->ip.port;	/* And destination port */
        setup->p1->len = strlen((char*)setup->p1->data) + 1;
        SDLNet_UDP_Send(setup->sd1, -1, setup->p1);

 

        Gupd->change_flag = 0;
    }

        if (SDLNet_UDP_Recv(setup->sd2, setup->p2)) {

        int a, b;
        sscanf_s((char*)setup->p2->data, "%d %d\n", &a, &b);
        //printf("%d %d\n", a, b);
        Gupd->opponent_position_x = a;
        Gupd->opponent_position_y = b;
         }


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



int SetPlayerPosX(Game_State Gupd, long int Player_posX)
{
    
    if (Gupd->Player_position_x != Player_posX) {

        Gupd->change_flag = 1;
        Gupd->Player_position_x = Player_posX;
    }
    return 0;
}

int SetPlayerPosY(Game_State Gupd, long int Player_posY)
{
    if (Gupd->Player_position_y != Player_posY) {

        Gupd->change_flag = 1;
        Gupd->Player_position_y = Player_posY;
    }
    return 0;
}
