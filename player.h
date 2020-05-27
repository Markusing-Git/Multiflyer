#ifndef player_h
#define player_h
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <time.h>
#include "constants.h"
#include "loadMedia.h"

//statemachine for different powerUp modes
typedef enum powerType {
	life,
	shield,
	attack,
	coin,
	none
} PowerType;

//statemachine for player skin
typedef enum skin_choices {
	fly,
	horned,
	goggle,
	angry
} SkinChoices;


typedef struct playerType* Player;


/*************** Constructors ***********************************/

//creates a player struct
PUBLIC Player createPlayer(int x, int y);

//creates a new player and adds to the list of players
PUBLIC void initPlayers(Player playerList[], int playerCount);



/*************** Get and setters ***********************************/

//returns adress of player position 
PUBLIC SDL_Rect* getPlayerPosAdr(Player aPlayer);

//returns player players status false: dead true: alive
PUBLIC bool getPlayerStatus(Player aPLayer);

//sets the status of player false: dead true: alive
PUBLIC void setPlayerStatus(Player aPlayer, bool deadOrAlive);

//returns player resurection status: true if resurected false: dead or alive
PUBLIC bool getPlayerResurect(Player aPlayer);

//sets players resurected status
PUBLIC void setPlayerResurect(Player aPlayer, bool resurected);

//returns the consumed power of player
PUBLIC int getPlayerPower(Player aPlayer);

//sets the consumed power of player
PUBLIC void setPlayerPower(Player aPlayer, PowerType aPowerType);

//returns true if player is immune of obstacles and false if not.
PUBLIC bool getPlayerImmunity(Player aPlayer);

//sets cordinates of a players x:y:w:h, value represents the value to be set
PUBLIC void setPlayerPoint(Player aPlayer, char cord, int value);

//returns the value of player cordinate. cord: x:y:w:h
PUBLIC int getPlayerPoint(Player aPlayer, char cord);

//Returns the score of a player
PUBLIC int getPlayerScore(Player aPlayer);

//returns number of coins player has accumulated
PUBLIC int getPlayerCoins(Player aPlayer);

//sets player coins
PUBLIC void setPlayerCoins(Player aPlayer, int coins);

//sets player choice of skin
PUBLIC void setPlayerSkin(Player aPlayer, SkinChoices aChoice);

//returns players skin
PUBLIC int getPlayerSkin(Player aPlayer);

//Plays the sound effect
void getSoundEffect(int alive, Mix_Chunk* soundEffect);

// check if space button is pressed. 
PUBLIC bool getPlayerAttack(Player aPlayer);

// set attack to true if space is pressed.
PUBLIC void setPlayerAttack(Player aPlayer, bool attackOrNot);



/*************** Operational ***************************************/

//renders players dead or alive
PUBLIC void renderPlayers(SDL_Renderer* renderer, Player playerList[], int playerFrame, int splashFrame[], int* nrOfSoundEffects, int playerCount, LoadMedia media);

//Renders player score
PUBLIC void renderScore(Player aPlayer, LoadMedia media, SDL_Renderer* renderer, Fonts fonts);

//Renders playerpower effects
PUBLIC void renderPlayerPower(SDL_Renderer* renderer, LoadMedia media, Player playerList[], int localPlayer, int playerCount);

//renders immunity bar if player is immune
PUBLIC void renderImmunityBar(SDL_Renderer* renderer, LoadMedia media, Player aPlayer, int* immunityFrames);

//moves a player
PUBLIC void movePlayerUp(Player aPlayer, int speed);
PUBLIC void movePlayerDown(Player aPlayer, int speed);
PUBLIC void movePlayerLeft(Player aPlayer, int speed);
PUBLIC void movePlayerRight(Player aPlayer, int speed);

//Adds one to a players score
PUBLIC void addScore(Player aPlayer);

//Checks if there is contact between players in case of a push
PUBLIC int playerContact(SDL_Rect* playerPos, SDL_Rect* opponentPos);

//handles all player power related functions
PUBLIC void handlePlayerPowers(Player aPlayer, Uint32* resurectTimer, Uint32* immunityTimer, Uint32* powerDurationTimer, int* nrOfSoundEffects);

/*checks if all players are dead and game is over,
params: list of players and how many
Uint32 delay for lastplayer splash effects
*/
PUBLIC bool gameOver(Player playerList[], int playerCount, Uint32* delay, bool* delayFlag);

//Checks if there is contact between players in case of a push
PUBLIC int playerContact(SDL_Rect* playerPos, SDL_Rect* opponentPos);

//render attack
PUBLIC void renderAttack(SDL_Renderer *renderer, LoadMedia media, Player playerList[], int playerCount, int attackFrame[]);



/*************** Destructors **************************************/

//frees player structs from heap, params: list of players and amount of players
PUBLIC void freePlayers(Player playerList[], int playerCount);

#endif /* player_h */