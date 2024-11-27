/*************
 * spectator.h file for nuggets game
 * 
 * 
 * Leah Branstetter
 * team Big D Nuggets
 */

#ifndef SPECTATOR_H
#define SPECTATOR_H

#include "log.h" 
#include "message.h"

/***************** structs **********/
typedef struct spectator {
    addr_t IPaddress;
} spectator_t;

/****************** functions  ********/

/**************** spectator_new *******
 * 
 * This takes in the address of the client
 * it creates a new spectator struct which is pretty much just a wrapper for an IPaddress
 * 
 * it returns the spectator struct
 * 
 * assumes that the 
*/
spectator_t* spectator_new(const addr_t address);
const addr_t spectator_getAddress(spectator_t* spectator);
bool spectator_sendMessage(spectator_t* player, const char* message);
void spectator_delete(spectator_t* spectator);

#endif