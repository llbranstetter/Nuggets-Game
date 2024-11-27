#include <stdio.h>
#include <stdlib.h>
#include "grid.h"
#include "../support/log.h"
#include "../support/file.h"

int main() {
    // Initialize logging to stderr
    log_init(stderr);

    // Load the grid from a sample map file
    grid_t* mainGrid = grid_load("../maps/main.txt");
    grid_t* originalGrid = grid_load("../maps/main.txt");
    if (mainGrid == NULL) {
        printf("Failed to load grid.\n");
        return 1;
    }

    printf("Loaded main grid:\n");
    grid_print(mainGrid);
    printf("Rows: %d, Cols: %d\n", mainGrid->nrow, mainGrid->ncol);

    // Test creating a player grid
    grid_t* playerGrid = grid_playerLoad(mainGrid);
    if (playerGrid == NULL) {
        printf("Failed to initialize player grid.\n");
        grid_delete(mainGrid);
        return 1;
    }
    printf("Loaded playerGrid\n");

    // Set a player position for visibility testing
    int player_r = 8;  // example row for player start
    int player_c = 44;  // example column for player start

    // Calculate visibility for the player
    grid_calculateVis(mainGrid, playerGrid, originalGrid, player_r, player_c);

    grid_print(playerGrid);

    // Test wall detection
    printf("\nTesting grid_isWall:\n");
    if (grid_isWall(mainGrid, player_r, player_c)) {
        printf("Position (%d, %d) is a wall.\n", player_r, player_c);
    } 
    else {
        printf("Position (%d, %d) is not a wall.\n", player_r, player_c);
    }

    // Test gold detection
    printf("\nTesting grid_isGold:\n");
    if (grid_isGold(mainGrid, player_r, player_c)) {
        printf("Position (%d, %d) contains gold.\n", player_r, player_c);
    } 
    else {
        printf("Position (%d, %d) does not contain gold.\n", player_r, player_c);
    }

    // Test valid start position detection
    printf("\nTesting grid_validStart:\n");
    if (grid_validStart(mainGrid, player_r, player_c)) {
        printf("Position (%d, %d) is a valid start.\n", player_r, player_c);
    } 
    else {
        printf("Position (%d, %d) is not a valid start.\n", player_r, player_c);
    }

    printf("Running grid_toString test...\n");
    char gridAsString[1700];
    grid_toString(playerGrid, gridAsString);
    printf("Testing grid_toString:\n%s\n", gridAsString);

    // Clean up
    grid_delete(mainGrid);
    grid_delete(originalGrid);
    grid_delete(playerGrid);
    log_done();

    return 0;
}