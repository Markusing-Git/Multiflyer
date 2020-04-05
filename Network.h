#ifndef Network_h
#define Network_h


#define NAME_LENGTH 100
#define IP_LENGHT 16
#define _CRT_SECURE_NO_DEPRECATE


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
int SetPlayerPosX(Game_State Gupd,int Player_posX);
int SetPlayerPosY(Game_State Gupd,int Player_posY);
int getOpponentPosX(Game_State Gupd);
int getOpponentPosY(Game_State Gupd);
int getPlayerPosX(Game_State Gupd);
int getPlayerPosY(Game_State Gupd);


#endif /*Network_h*/