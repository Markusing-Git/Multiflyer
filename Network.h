#ifndef Network_h
#define Network_h


#define IP_LENGHT 16
#define _CRT_SECURE_NO_DEPRECATE
#define MAX_TEXT_LENGHT 100


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "obstacle.h"
#include "player.h"


struct UDP_Config_Type{
    UDPsocket sd1;
    UDPsocket sd2;
    IPaddress ip;
    UDPpacket *p1;
    UDPpacket *p2;
    int port;

};


struct Game_State_Type
{
    int Player_position_x;
    int Player_position_y;
    int opponent_position_x;
    int opponent_position_y; //De nuvarande Positionerna som
    bool player_alive;
    bool opponent_alive;
    int change_flag;
    int obstacle_change_flag;
    SDL_Rect obstacle_top;
    SDL_Rect obstacle_bottom;
}; 


typedef struct UDP_Config_Type* UDP_Config;
typedef	struct Game_State_Type* Game_State;
typedef	struct Player_Name_Type* Player_Name;

int create_Game_state(int Player_position_x, int Player_position_y, Game_State Gupd);
int sendAndRecive(Game_State Gupd, UDP_Config setup, SDL_Rect *local_player, SDL_Rect *local_opponent);
int SetPlayerAlive(Game_State Gupd, bool Alive_Local);
int updateGameSending(Game_State Gupd, SDL_Rect* local_player);
int networkCommunication(Game_State Gupd, UDP_Config setup);
int updateGameReciving(Game_State Gupd, SDL_Rect* Local_opponent);
int int_network(char IP_input[IP_LENGHT], int port, UDP_Config setup);
int Close_SDLNet(UDP_Config setup);
int SetPlayerPosX(Game_State Gupd,int Player_posX);
int SetPlayerPosY(Game_State Gupd,int Player_posY);
int getOpponentPosX(Game_State Gupd);
int getOpponentPosY(Game_State Gupd);
int getPlayerPosX(Game_State Gupd);
int getPlayerPosY(Game_State Gupd);
int SetObstacle(Game_State Gupd, Obstacle Send_obstacles);
int waitForConnection();
int establishConnection();
Obstacle ReciveObstacle(Game_State Gupd);


#endif /*Network_h*/