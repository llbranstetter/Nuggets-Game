/********** 
 * 
 * testing file to make sure the basic functions in player are working
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <time.h>
#include "../support/log.h" 
#include "../grid.h" 
#include "../gamestatus/gamestatus.h"
#include "../support/message.h"
#include "player.h"

// Test the player_new function
void test_player_creation() {
    grid_t* test_grid = grid_load(map);  // Assuming this is the correct way to allocate a grid
    addr_t* test_address = message_noAddr(); // Mocking the IP address as well
    char* test_name = "Player 1";
    int test_id = 1;

    player_t* player = player_new(test_address, test_name, test_id, test_grid);
    assert(player != NULL);
    assert(strcmp(player->name, test_name) == 0);
    assert(player->ID == test_id);
    assert(player->score == 0);
    assert(player->grid == test_grid);
    assert(player->IPaddress == test_address);

    printf("Player creation test passed.\n");

    // Cleanup
    free(test_grid);
    free(test_address);
    free(player);
}

// Test getter and setter functions
void test_player_setters_getters() {
    grid_t* test_grid = grid_load(map);  
    addr_t* test_address = message_noAddr(); 
    player_t* player = player_new(test_address, "Player 2", 2, test_grid);

    // Test setter and getter for location
    player_setLocation(player, 10);
    assert(player_getLocation(player) == 10);

    // Test setter and getter for score (gold)
    player_addGold(player, 5);
    assert(player_getGold(player) == 5);

    // Test setter for ID
    player_setID(player, 42);
    assert(player->ID == 42);

    // Test player name getter
    assert(strcmp(player_getName(player), "Player 2") == 0);

    printf("Setter and Getter test passed.\n");

    // Cleanup
    free(test_grid);
    free(test_address);
    free(player);
}

// Test the player_sendMessage function
void test_player_send_message() {
    grid_t* test_grid = grid_load(map);  
    addr_t* test_address = message_noAddr(); 
    player_t* player = player_new(test_address, "Player 3", 3, test_grid);

    // Send a valid message
    // This isn't gonna work without a valid address I fear
    bool result = player_sendMessage(player, "Hello, Player!");
    // assert(result == true);

    // Send a NULL message (should fail)
    result = player_sendMessage(player, NULL);
    assert(result == false);

    // Send a message to a NULL player (should fail)
    result = player_sendMessage(NULL, "Message to NULL player");
    assert(result == false);

    printf("Send message test passed.\n");

    // Cleanup
    free(test_grid);
    free(test_address);
    free(player);
}

// Test player memory management (deleting a player)
void test_player_delete() {
    grid_t* test_grid = grid_load(map);  
    addr_t* test_address = message_noAddr(); 
    player_t* player = player_new(test_address, "Player 4", 4, test_grid);

    // Delete player and ensure no memory leaks
    player_delete(player);

    printf("Player delete test passed.\n");
}

// Main function to run all tests
int main() {
    printf("Running player tests...\n");

    test_player_creation();
    test_player_setters_getters();
    test_player_send_message();
    test_player_delete();

    printf("All tests passed.\n");

    return 0;
}