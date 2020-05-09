#ifndef constants_h
#define constants_h

#define PLAYER_FRAMES 2 //Antal frames i spritesheet
#define SPLASH_FRAMES 14
#define MAX_PLAYERS 4
#define TIME_DELAY 1600 //A time delay based on milliseconds
#define POWERUP_TIME_DELAY 10000 //A Time delay for spawning powerups based on milliseconds
#define IP_LENGTH 16
#define NAME_LENGTH 100

#define PUBLIC			//empty for public variables or functions
#define PRIVATE static  //static for private variables or fuctions

typedef enum { singlePlayerRoute, clientRoute, hostRoute, menuRoute, quitRoute } Game_Route;

#endif /* constants_h */