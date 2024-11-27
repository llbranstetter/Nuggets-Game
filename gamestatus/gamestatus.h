/*************
 * gamestatus.h header file for gamestatus.c
 * 
 * Kellen Seeley
 * Group Big D Nuggets
 * Nuggets final project gamestatus module
 */
#ifndef GAMESTATUS_H
#define GAMESTATUS_H


#include "../support/message.h"
#include "../grid/grid.h"
#include "../gold/gold.h"
#include "../clienttypes/player.h"
#include "../clienttypes/spectator.h"
#include "../support/log.h"

// static const int MaxPlayers = 26;      // maximum number of players
#define MaxPlayers 26  


/************* structs ************/
/**
 * Represents the overall game state.
 */
typedef struct gamestatus {
    grid_t* grid;           // pointer to the main game grid
    grid_t* originalGrid;   // pointer to the original grid layout
    gold_t** goldPiles;     // array of pointers to gold piles
    player_t* players[MaxPlayers];    // array of pointers to players in the game
    spectator_t* spectator;   // pointer to the spectator
    int totalGold;   // total remaining gold in the game
    int numPlayers;  // current number of players in the game
    int numGoldPiles;   // number of gold piles in the game
    bool gameOver;   // boolean to track if the game is over
} gamestatus_t;

/************* functions ************/

/**************** gamestatus_new *****************/
/**
 * initializes a new gamestatus_t struct with a map file and seed
 * loads the grid, sets up gold piles, and initializes players and spectator pointers
 * 
 * @param mapFile the pointer to the map to load.
 * @param seed the seed for random number generation.
 * @return a pointer to the newly created gamestatus_t, or NULL on failure.
 */
 gamestatus_t* gamestatus_new(const char* mapFile, int seed);

/**************** gamestatus_addPlayer *****************/
/**  adds a player to the game, returning true if successful, false if player limit reached, assigns them a unique ID and position
* 
 * @param game the current game state
 * @param playerName the name of the player to add
 * @param address the network address of the player
 * @return a pointer to the new player_t if successful, NULL if the player could not be added
*/

player_t* gamestatus_addPlayer(gamestatus_t* game, char* playerName, const addr_t address);

/**************** gamestatus_addSpectator *****************/
/**  adds a spectator to the game if no spectator is currently present
*
* @param game the current game state
* @param address the network address of the spectator being added
* @return true if the spectator was successfully added, false otherwise
*/
bool gamestatus_addSpectator(gamestatus_t* game, const addr_t address);

/**************** gamestatus_distributeGold *****************/
/**  distributes gold piles randomly across floor cells within the grid
*
* @param game the current game state.
* @param minPiles the minimum number of gold piles to distribute.
* @param maxPiles the maximum number of gold piles to distribute.
*/
void gamestatus_distributeGold(gamestatus_t* game, int minPiles, int maxPiles);

/**************** gamestatus_getPlayerByAddress *****************/
/** finds and returns a pointer to the player with a matching network address, or NULL if not found
*
* @param game the current game state
* @param address the network address being searched for
* @return a pointer to the matching player_t, or NULL if no match is found
*/
player_t* gamestatus_getPlayerByAddress(gamestatus_t* game, const addr_t address);

/**************** gamestatus_removePlayer *****************/
/**  removes a player by their network address from the game
*
* @param game the current game state
* @param address the network address of the player to remove
*/
void gamestatus_removePlayer(gamestatus_t* game, const addr_t address);

/**************** gamestatus_removeSpectator *****************/
/** removes the spectator from the game
*
* @param game the current game state.
*/
void gamestatus_removeSpectator(gamestatus_t* game);

/**************** gamestatus_checkGameOver *****************/
/**  checks if the game is over by verifying if all gold has been collected, calls gamestatus_endGame if so
*
* @param game the current game state
* @return true if the game is over, false otherwise
*/
bool gamestatus_checkGameOver(gamestatus_t* game);

/**************** gamestatus_endGame *****************/
/** ends the game by sending a quit message to each player and spectator
* 
* @param game the current game state
*/
void gamestatus_endGame(gamestatus_t* game);

/**************** gamestatus_delete *****************/
/** frees all memory associated with the gamestatus structure
* 
* @param game the current gamestate to delete
*/
void gamestatus_delete(gamestatus_t* game);

#endif 