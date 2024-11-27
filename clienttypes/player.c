/*************
 * player.c file for nuggets game
 * 
 * defines player struct
 * Leah Branstetter
 * team Big D Nuggets
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <time.h>
#include "../support/log.h" 
#include "../grid/grid.h" 
#include "../support/message.h"
#include "player.h"

/************* global functions ********/

/******** player_new 
 * see player.h for documentation
*/
player_t* player_new(const addr_t address, char* name, int ID, grid_t* grid){
    player_t* player = malloc(sizeof(player_t));
    
    if (player == NULL){
        flog_s(stderr, "%s", "Failed to allocate memory for new player.");
        return NULL;
    }
    
    int r;
    int c;
    int ncol;
    int nrow;
    bool valposition = false;
    while (!valposition){ // find a valid postion:
        nrow = grid_getRows(grid);
        ncol = grid_getCols(grid);
        r = rand() % nrow;  // Random row choice
        c = rand() % ncol;  // Random column choice
        valposition = grid_validStart(grid, r, c);
    }
    player->grid = grid_playerLoad(grid);
    player->name = name;
    player->IPaddress = address;
    player->ID = ID;
    player->position = r * (ncol+1) + c;
    player->score = 0;
    player->isPlaying = true;

    return player;
    
}

/******** player_setLocation
 * see player.h for documentation
*/
void player_setLocation(player_t* player, int position){
    player->position = position;
}

/******** player_getLocation
 * see player.h for documentation
*/
int player_getLocation(player_t* player){
    return player->position;
}

void player_setGrid(player_t* player, grid_t* newgrid){
    player->grid = newgrid;
}

grid_t* player_getGrid(player_t* player){
    return player->grid;
}

/******** player_setID
 * see player.h for documentation
*/
void player_setID(player_t* player, int ID){
    if (ID < 0 || player == NULL){
        return;
    }
    player->ID = ID;
}

/******** player_addGold
 * see player.h for documentation
*/
void player_addGold(player_t* player, int value){
    player->score = player->score + value;
}

/******** player_setGold
 * see player.h for documentation
*/
void player_setGold(player_t* player, int score){
    if (score < 0 || player == NULL){
        return;
    }
    player->score = score;
}

/******** player_getGold
 * see player.h for documentation
*/
int player_getGold(player_t* player){
    return player->score;
}

/******** player_getName
 * see player.h for documentation
*/
char* player_getName(player_t* player){
    return player->name;
}

/************* player_sendMessage
 * 
 * sends a message to the player's IPaddress
 * checks that neither the message or the address are NULL
 * if they are NULL, return false. Otherwise send the message and return true
 */
bool player_sendMessage(player_t* player, char* message){
    if(player == NULL || message == NULL){
        return false;
    }
    addr_t paddr = player->IPaddress;
    message_send(paddr, message);
    return true;
}

/******** player_delete
 * see player.h for documentation
*/
void player_delete(player_t* player){
    if (player == NULL){
        return;
    }
    else{
        free(player->name);
        free(player);
    }
}

