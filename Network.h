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
#include "world.h"

//f�r att du ska f� tillg�ng till Lobby, funktioner m�ste skapas f�r att utf�ra operationer.
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

struct Game_State_Type
{
    int player_Pos_X[MAX_PLAYERS];
    int player_Pos_Y[MAX_PLAYERS];
    bool player_Alive[MAX_PLAYERS];
    char playerNames[4][NAME_LENGTH];
    char ipAdressCache[IP_LENGTH];
    int pushAngle[MAX_PLAYERS];

    int playerScore[MAX_PLAYERS];
    int nrOfPlayers;
    int change_flag;
    int obstacle_change_flag;
    int powerUp_change_flag;
    int lobbyRunningFlag;
    int newPlayerFlag;
    int localPlayerNr;

    SDL_Rect obstacle_top;
    SDL_Rect obstacle_bottom;

    SDL_Rect powerUpRect;
    int powerUpDir;
    int powerUpType;
    int resurected[MAX_PLAYERS];
    int playerPower[MAX_PLAYERS];
    bool attack[MAX_PLAYERS];
    bool attackPower;

}; 
typedef	struct Game_State_Type* Game_State;

struct Game_State_Send_Type
{
    int player_Pos_X;
    int player_Pos_Y;
    int playerScore;
    bool player_Alive;
    int pushAngle[MAX_PLAYERS];
    int resurected;
    int playerPower;
    bool attack;
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
int resetServerSDLNet(UDP_Client_Config setup, Game_State current);
int resetClientSDLNet(UDP_Client_Config setup);

int SetGameStatePlayerStatus(Game_State current, Player players[]);
int SetGameStatePlayerPosX(Game_State current, SDL_Rect* playerPos[]);
int SetGameStatePlayerPosY(Game_State current, SDL_Rect* playerPos[]);
int getGameStatePlayerPosX(Game_State current, int playerNr);
int getGameStatePlayerPosY(Game_State current, int playerNr);
int SetObstacle(Game_State Gupd, Obstacle Send_obstacles);
void SetPowerUp(Game_State current, PowerUp aPowerUp);

int serverLobbyConnection(Game_State current); 
int clientLobbyConnection(char playerIp[], char playerName[], Game_State current);
int clientLobbyWait(Game_State current);
int serverSendPlayer(char playerIp[], char playerName[], int localPlayerNr, Game_State current);

Obstacle ReciveObstacle(Game_State Gupd);
PowerUp ReceivePowerUp(Game_State current);


#endif /*Network_h*/