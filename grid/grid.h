/*
 * grid.h - grid header file for the nuggets program
 *
 * This module creates the data structures necessary
 * to keep track of everything happening on the map
 * by storing every point in an array. We also use 
 * a struct to keep track of the visibility of each 
 * player in a similar manor. 
 * 
 * Team Big D Nuggies 
 * Jake Fleming, Fall 2024
 */
#ifndef GRID_H
#define GRID_H
#include <stdbool.h>

/************ Global Structures **************/
typedef struct grid {
    char* gridArray;
    int ncol;
    int nrow;
} grid_t;

/************ Global Functions **************/

/**************** grid_load *****************/
/*
 * Load the main grid for the game 
 *
 * Inputs:
 *   mapFile - pathname to the map text file
 * 
 * Output:
 *   intialized grid_t struct for game
 *
 * We do:
 *   open and read file, adding number of 
 *   rows, columns, and an array to the struct 
 * 
 * User is responsible for freeing later
 */
grid_t* grid_load(const char* mapFile);


/**************** grid_playerLoad *****************/
/* 
 * Load a new grid struct for a player that joins
 *
 * Inputs:
 *   mainGrid - pointer to main grid struct 
 * 
 * Output:
 *   intialized grid_t struct for player
 * 
 * We do:
 *   copy grid elements into a new grid, but 
 *   instantiating all the spaces to blanks
 * 
 * User is responsible for freeing later
 */
grid_t* grid_playerLoad(grid_t* mainGrid);


/**************** grid_delete ****************/
/* 
 * Free the memory allocated for a grid struct
 *
 * Inputs:
 *   grid - pointer to grid to delete
 */
void grid_delete(grid_t* grid);


/**************** grid_validStart ****************/
/*
 * Check if a point is valid start ('.') for player
 * 
 * Inputs:
 *   grid - pointer to grid struct
 *   r - row position
 *   c - column position
 * 
 * Output:
 *   true if valid, false otherwise
 */
bool grid_validStart(grid_t* grid, int r, int c);


/**************** grid_isWall ****************/
/*
 * Check if a point is wall ('-', '|', '+')
 * 
 * Inputs:
 *   grid - pointer to grid struct
 *   r - row position
 *   c - column position
 * 
 * Output:
 *   true if wall, false otherwise
 */
bool grid_isWall(grid_t* grid, int r, int c);

/**************** grid_isGold ****************/
/*
 * Check if a point is wall ('*')
 * 
 * Inputs:
 *   grid - pointer to grid struct
 *   r - row position
 *   c - column position
 * 
 * Output:
 *   true if wall, false otherwise
 */
bool grid_isGold(grid_t* grid, int r, int c);

/**************** grid_getCell ****************/
/*
 * Provide caller the character at a cell
 * 
 * Inputs:
 *   grid - pointer to grid struct
 *   r - row position
 *   c - column position
 * 
 * Output:
 *   character at the given position
 */
char grid_getCell(grid_t* grid, int r, int c);


/**************** grid_getRows ****************/
/*
 * Provide caller the number of rows
 * 
 * Inputs:
 *   grid - pointer to grid struct
 * 
 * Output:
 *   integer value of row count
 */
int grid_getRows(grid_t* grid);


/**************** grid_getCols ****************/
/*
 * Provide caller the number of columns
 * 
 * Inputs:
 *   grid - pointer to grid struct
 * 
 * Output:
 *   integer value of column count
 */
int grid_getCols(grid_t* grid);

/**************** is_within_bounds ****************/
/* 
 * Ensure we are not accessing a nonexistant position in
 * a given gridArray
 *
 * Inputs:
 *   grid - pointer to grid struct
 * 
 * Output:
 *   true if in bounds, false otherwise
 */
bool is_within_bounds(grid_t* grid, int row, int col);

/************** grid_calculateVis ***************/
/* 
 * main function to update visibility struct 
 *
 * Inputs: 
 *   mainGrid - pointer to grid struct
 *   playerGrid - pointer to player grid struct
 *   vis - pointer to vis struct
 *   player_r - row position of player
 *   player_c - column position of player
 *   
 * We do:
 *   Calculate line of sight from player to every
 *   point on grid
 */
void grid_calculateVis(grid_t* mainGrid, grid_t* playerGrid, grid_t* originalGrid, int player_r, int player_c);

/************** line_of_sight ***************/
/*
 * Use Breshenham's line algorithm to calculate LOS
 *
 * Inputs: 
 *   mainGrid - pointer to grid struct
 *   startRow - row position of player
 *   startCol - column position of player
 *   endRow - row position for LOS
 *   endCol - column position for LOS
 * 
 * Output:
 *   true if point is in LOS, false otherwise
 * 
 * We do:
 *   check where line intersects each row and column
 *   and determine if visibility continues
 */
bool line_of_sight(grid_t* mainGrid, int startRow, int startCol, int endRow, int endCol);

void grid_addGoldPile(grid_t* grid, int r, int c);

void grid_removeGoldPile(grid_t* grid, int r, int c);


/************** grid_print ***************/
/* 
 * Print the entire grid
 *
 * Inputs:
 *   grid - pointer to grid struct
 * 
 * We do:
 *   loop through every cell and print the
 *   character
 */
void grid_print(grid_t* grid);

/************** grid_toString ***************/
/* 
 * return a string of entire grid
 *
 * Inputs:
 *   grid - pointer to grid struct
 * 
 * Output:
 *   String of entire grid
 * 
 * We do:
 *   Create a buffer of size message_MaxBytes
 *   and start the string with DISPLAY\n so 
 *   server can easily send it as message
 */
void grid_toString(grid_t* grid, char* message);

#endif