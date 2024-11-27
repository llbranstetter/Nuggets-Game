/*************
 * player.h file for player functions
 * 
 * Leah Branstetter
 * Group Big D Nuggets
 */
#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "../support/log.h" 
#include "../grid/grid.h" 
#include "../support/message.h"

/************* structs ***********/
typedef struct player {
    char* name;
    int ID; // unique ID used to identify a player
    int position;  // player position
    int score; // Amount of gold collected by the player
    bool isPlaying;
    addr_t IPaddress;
    grid_t* grid;
} player_t;

/*************** functions *******/

/************ player_new ********
 * 
 * function to create a new player struct and initialize it to a new location on the grid
 * initializes player score to 0, and player position to a random location
 * initializes the the parameters to player attributes
 * 
 * takes a valid addr_t struct, a player name, an intID, and a valid grid_t struct
 * 
 * returns a player struct or NULL if it cannot create a new player 
*/
player_t* player_new(const addr_t address, char* name, int ID, grid_t* grid); // grid used to set location randomly

void player_setLocation(player_t* player, int position); // update location
int player_getLocation(player_t* player); //return location
void player_setGrid(player_t* player, grid_t* newgrid); 
grid_t* player_getGrid(player_t* player);
void player_setID(player_t* player, int ID); // set player ID
void player_addGold(player_t* player, int value); // update gold
void player_setGold(player_t* player, int score); // set the players score to a specific value
int player_getGold(player_t* player);  //return gold
char* player_getName(player_t* player); // return player's name
bool player_sendMessage(player_t* player, char* message);
void player_delete(player_t* player); // cleanup

#endif
