/*************
 * spectator.c file for nuggets game
 * 
 * defines spectator struct
 * Leah Branstetter
 * team Big D Nuggets
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <time.h>
#include "log.h" 
#include "message.h"
#include "spectator.h"


/************* global functions ********/

/******** spectator_new 
 * see spectator.h for documentation
*/
spectator_t* spectator_new(const addr_t address){
    spectator_t* spectator = malloc(sizeof(spectator_t));
    if (spectator == NULL){
        flog_s(stderr, "%s", "Failed to allocate memory for new spectator.");
        return NULL;
    }
    else{
        spectator->IPaddress = address;
    }
    return spectator;
}

/************* spectator_getAddress
 * see spectator.h for documentation
 */
const addr_t spectator_getAddress(spectator_t* spectator){
    return spectator->IPaddress;
}

/********************* spectator_sendMessage
 * see spectator.h for documentation
 */
bool spectator_sendMessage(spectator_t* player, const char* message){
    if (player == NULL || message == NULL){
        return false;
    }
    message_send(player->IPaddress, message);
    return true;
}

void spectator_delete(spectator_t* spectator){
    free(spectator);
}