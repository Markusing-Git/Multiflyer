#ifndef Network_h
#define Network_h

#ifndef _MSC_VER

#define scanf_s( fmt, ... ) scanf( scanf_validate( fmt, __FILE__, __LINE__ ), __VA_ARGS__ )

const char* scanf_validate(const char* fmt, const char* file, long line)
{
    const char* p = fmt;
    while (1)
    {
        p = strstr(p, "%s");
        if (p == NULL) break;
        if ((p == fmt) || (*(p - 1) != '%'))
        {
            fprintf(stderr, "Hey, you used \"%%s\" in %s: line %d!\n", file, line);
            abort();
        }
    }
    return fmt;
}

#endif 

#define NAME_LENGTH 100
#define IP_LENGHT 16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>


struct UDP_Config_Type{
    UDPsocket sd1;
    UDPsocket sd2;
    UDPsocket sd3;
    IPaddress ip;
    UDPpacket *p1;
    UDPpacket *p2;
    int port;
    int port_Set_Flag;

};

struct Player_Name_Type {
    char Player_name[NAME_LENGTH]; //Namnet som kommer att skickas
};

struct Game_State_Type
{
    int Player_position_x;
    int Player_position_y;
    int opponent_position_x;
    int opponent_position_y; //De nuvarande Positionerna som
    int change_flag;
    int setup_flag;
}; 


typedef struct UDP_Config_Type* UDP_Config;
typedef	struct Game_State_Type* Game_State;
typedef	struct Player_Name_Type* Player_Name;

int create_Game_state(int Player_position_x, int Player_position_y, Game_State Gupd);
int sendAndRecive(Game_State Gupd, UDP_Config setup);
int int_network(char IP_input[IP_LENGHT], int port, UDP_Config setup);
int Close_SDLNet(UDP_Config setup);
int SetPlayerPosX(Game_State Gupd, long int Player_posX);
int SetPlayerPosY(Game_State Gupd, long int Player_posY);
int getOpponentPosX(Game_State Gupd);
int getOpponentPosY(Game_State Gupd);
int getPlayerPosX(Game_State Gupd);
int getPlayerPosY(Game_State Gupd);


#endif /*Network_h*/