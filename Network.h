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

//för att du ska få tillgång till Lobby, funktioner måste skapas för att utföra operationer.
typedef struct lobby_type* Lobby;

struct UDP_Client_Config_Type{
    UDPsocket send_Sock;
    UDPsocket recv_Sock[MAX_PLAYERS];
    IPaddress sendingIP[MAX_PLAYERS];
    char playerIp[MAX_PLAYERS][IP_LENGTH];
    UDPpacket *send_Pack;
    UDPpacket *recv_Pack;
};
typedef struct UDP_Client_Config_Type* UDP_Client_Config;

struct Game_State_Type
{
    int playerPosX[MAX_PLAYERS];
    int playerPosY[MAX_PLAYERS];
    bool playerAlive[MAX_PLAYERS];
    char playerNames[4][NAME_LENGTH];
    //char *ipAdressCache; //Test Snabbare koppling
    char ipAdressCache[IP_LENGTH];
    int pushAngle[MAX_PLAYERS];
    int connectionTimers[MAX_PLAYERS];
    int disconnectionCache;
    int serverConnection;

    int playerScore[MAX_PLAYERS];
    int nrOfPlayers;
    int changeFlag;
    int obstacleChangeFlag;
    int powerUpChangeFlag;
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
    int playerSkin[MAX_PLAYERS];
    bool attack[MAX_PLAYERS];
    bool attackPower;

}; 
typedef	struct Game_State_Type* Game_State;

struct Game_State_Send_Type
{
    int playerPosX;
    int playerPosY;
    int playerScore;
    bool playerAlive;
    int pushAngle[MAX_PLAYERS];
    int resurected;
    int playerPower;
    int localPlayerNr;
    int playerSkin;
    bool attack;
};
typedef	struct Game_State_Send_Type* Game_State_Send;

struct TCP_Communication_Type
{
    char playerName[NAME_LENGTH];
    int recived;
    int startGame;
    int leftGame;
    int connectionOpen;
    int localPlayerNr;
    int serverDisconnect;
};
typedef	struct TCP_Communication_Type * TCP_Communication;

int start_Game_state(Player playerList[], Game_State current);
int initGamestate(Game_State current);
int initTCPCom(TCP_Communication communication);

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
int sendToClient(TCP_Communication communication,char playerIp[], Game_State current);
void renderConnectionsServer(Game_State current);
int renderConnectionsClient(Game_State current);
void removePlayerFromLobby(Game_State current, UDP_Client_Config setup, int localPlayerNr);
int closeServer(Game_State current, TCP_Communication communication, UDP_Client_Config setup);
int disconnectFromServer(char playerIp[], Game_State current);

Obstacle ReciveObstacle(Game_State Gupd);
PowerUp ReceivePowerUp(Game_State current);


#endif /*Network_h*/