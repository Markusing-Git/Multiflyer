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

typedef struct lobby_type* Lobby;

//Struct with network critical variables
struct Network_Config_Type{
    UDPsocket send_Sock;
    UDPsocket recv_Sock[MAX_PLAYERS];
    IPaddress sendingIP[MAX_PLAYERS];
    char playerIp[MAX_PLAYERS][IP_LENGTH];
    UDPpacket *send_Pack;
    UDPpacket *recv_Pack;
};
typedef struct Network_Config_Type* Network_Config;

//Game_State struct to track information currently on the network.
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

//Smaller version of Game_Sate sent by clients
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

//Struct with flags that are sent over TCP for communications
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


/***************Initiations***********************************/

//Sets Game_State with player data at start of game
void set_Game_state(Player playerList[], Game_State current);

//Initiates gamestate with default values
void initGamestate(Game_State current);

//Initiates TCP communications struct
void initTCPCom(TCP_Communication communication);

//Initiates the variables of the network struct for clients
void init_client_network(char playerIp[], Network_Config setup, Game_State current);

//Initiates the variables of the network struct for hosts
void init_Server_network(Network_Config setup, Game_State current);

//Resets the network to play again for server.
int resetServerSDLNet(Network_Config setup, Game_State current);

//Resets the network to play again for client.
int resetClientSDLNet(Network_Config setup);

/*************************************************************/


/***************UDP network communication***********************************/

//Updating and sending information from Game_State for Client 
int sendAndReciveClient(Game_State Gupd, Network_Config setup, SDL_Rect* playerPos[], Player players[]);

//Updating and sending information from Game_State for Host 
int sendAndReciveServer(Game_State Gupd, Network_Config setup, SDL_Rect* playerPos[], Player players[]);

//Updates the Game_State with current player information from the local player
int updateGameSending(Game_State current, SDL_Rect* playerPos[], Player players[]);

//Updates the Game_State with player information from the network
int updateGameReciving(Game_State current, SDL_Rect* playerPos[], Player players[]);

//Sending and reciving for client over the network to the host
int networkCommunicationClient(Game_State current, Network_Config setup);

//Sending and reciving for host over the network to all clients 
int networkCommunicationServer(Game_State current, Network_Config setup);

/***************************************************************************/

/***************UDP network communication***********************************/

int SetGameStatePlayerStatus(Game_State current, Player players[]);
int SetGameStatePlayerPosX(Game_State current, SDL_Rect* playerPos[]);
int SetGameStatePlayerPosY(Game_State current, SDL_Rect* playerPos[]);
int SetObstacle(Game_State Gupd, Obstacle Send_obstacles);
void SetPowerUp(Game_State current, PowerUp aPowerUp);
Obstacle ReciveObstacle(Game_State Gupd);
PowerUp ReceivePowerUp(Game_State current);

/***************TCP network communication***********************************/
int serverLobbyConnection(Game_State current); 
int clientLobbyConnection(char playerIp[], char playerName[], Game_State current);
int clientLobbyWait(Game_State current);
int sendToClient(TCP_Communication communication,char playerIp[], Game_State current);
void renderConnectionsServer(Game_State current);
int renderConnectionsClient(Game_State current);
void removePlayerFromLobby(Game_State current, Network_Config setup, int localPlayerNr);
int closeServer(Game_State current, TCP_Communication communication, Network_Config setup);
int disconnectFromServer(char playerIp[], Game_State current);


#endif /*Network_h*/