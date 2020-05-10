#ifndef Network_h
#define Network_h

#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "obstacle.h"
#include "player.h"
#include "constants.h"

//för att du ska få tillgång till Lobby, funktioner måste skapas för att utföra operationer.
typedef struct lobby_type* Lobby;

struct UDP_Client_Config_Type{
    UDPsocket send_Sock;
    UDPsocket recv_Sock[MAX_PLAYERS];
    IPaddress sendingIP[MAX_PLAYERS];
    char playerIp[MAX_PLAYERS][IP_LENGTH];
    UDPpacket *send_Pack;
    UDPpacket *recv_Pack;
    int port[MAX_PLAYERS];
};
typedef struct UDP_Client_Config_Type* UDP_Client_Config;

struct Player_info_Type
{
    int player_Pos_X;
    int player_Pos_Y;
    bool player_Alive;
};
typedef struct Player_info_Type* Player_info;

struct Game_State_Type
{
    int player_Pos_X[MAX_PLAYERS];
    int player_Pos_Y[MAX_PLAYERS];
    bool player_Alive[MAX_PLAYERS];
    char playerNames[4][NAME_LENGTH];
    char ipAdressCache[IP_LENGTH];
    int pushAngle[MAX_PLAYERS];

    int nrOfPlayers;
    int change_flag;
    int obstacle_change_flag;
    int lobbyRunningFlag;
    int newPlayerFlag;
    int localPlayerNr;

    SDL_Rect obstacle_top;
    SDL_Rect obstacle_bottom;
}; 
typedef	struct Game_State_Type* Game_State;

struct Game_State_Send_Type
{
    int player_Pos_X;
    int player_Pos_Y;
    bool player_Alive;
    int pushAngle[MAX_PLAYERS];
};
typedef	struct Game_State_Send_Type* Game_State_Send;


int start_Game_state(Player playerList[], Game_State current);
int initGamestate(Game_State current);

int sendAndReciveServer(Game_State Gupd, UDP_Client_Config setup, SDL_Rect* playerPos[], Player players[]);
int sendAndReciveClient(Game_State Gupd, UDP_Client_Config setup, SDL_Rect* playerPos[], Player players[]);
int updateGameSending(Game_State current, SDL_Rect* playerPos[], Player players[]);
int updateGameReciving(Game_State current, SDL_Rect* playerPos[], Player players[]);

int networkCommunicationServer(Game_State current, UDP_Client_Config setup);
int networkCommunicationClient(Game_State current, UDP_Client_Config setup);
int init_client_network(char playerIp[], UDP_Client_Config setup, Game_State current);
int init_Server_network(UDP_Client_Config setup, Game_State current);

int SetPlayerAlive(Game_State current, Player players[]);
int SetPlayerPosX(Game_State current, SDL_Rect* playerPos[]);
int SetPlayerPosY(Game_State current, SDL_Rect* playerPos[]);
int getPlayerPosX(Game_State current, int playerNr);
int getPlayerPosY(Game_State current, int playerNr);
int SetObstacle(Game_State Gupd, Obstacle Send_obstacles);

int serverLobbyConnection(Game_State current); 
int clientLobbyConnection(char playerIp[], char playerName[], Game_State current);
int clientLobbyWait(Game_State current);
int serverSendPlayer(char playerIp[], char playerName[], int localPlayerNr, Game_State current);
int serverStartGame(UDP_Client_Config setup, Game_State current);
int clientStartGame(Game_State current);

Obstacle ReciveObstacle(Game_State Gupd);


#endif /*Network_h*/