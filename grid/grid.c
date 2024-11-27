/*
 * grid.c - grid file for the nuggets program
 * 
 * Team Big D Nuggies 
 * Jake Fleming, Fall 2024
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include "../support/file.h"
#include "../support/log.h"
#include "../support/message.h"
#include "grid.h"

/*
 * CELL macros accesses cell position
 * from a given row and column.
 * 
 * Inputs:
 *   s - pointer to a grid/ struct
 *   r - row value
 *   c - column value
 */
#define CELL(grid,r,c) ((grid)->gridArray[(r)*((grid)->ncol + 1) + (c)])

/************ global functions *************/

/**************** grid_load *****************/
/* see grid.h for more detailed description */
grid_t*
grid_load(const char* mapFile) 
{
    FILE* file = fopen(mapFile, "r");
    if (file == NULL) {
        log_e("Error: Could not open map file");
        return NULL;
    }

    // get number of rows and check it isn't empty
    int rows = file_numLines(file);
    if (rows <= 0) {
        log_e("Error: Map file is empty or unreadable");
        fclose(file);
        return NULL;
    }

    // determine columns by reading first line and check it worked
    char* firstLine = file_readLine(file);
    if (firstLine == NULL) {
        log_e("Error: Could not read the first line of map file");
        fclose(file);
        return NULL;
    }
    int cols = strlen(firstLine);
    free(firstLine);

    // create grid structure
    grid_t* grid = malloc(sizeof(grid_t));
    if (grid == NULL) {
        log_e("Error: Could not allocate grid structure");
        fclose(file);
        return NULL;
    }
    // add data
    grid->nrow = rows;
    grid->ncol = cols;
    grid->gridArray = malloc(rows * (cols + 1) * sizeof(char));
    if (grid->gridArray == NULL) {
        log_e("Error: Could not allocate grid array");
        free(grid);
        fclose(file);
        return NULL;
    }

    // load grid layout into 1D array
    rewind(file); // reset file pointer
    for (int r = 0; r < rows; r++) {
        char* line = file_readLine(file);
        if (line == NULL || strlen(line) != cols) {
            log_d("Error reading line %d or inconsistent column count", r);
            free(line);
            break;
        }
        for (int c = 0; c < cols; c++) {
            CELL(grid, r, c) = line[c];
        }
        free(line);
    }
    fclose(file);
    log_s("Grid loaded from file: %s", mapFile);
    return grid;
}

/**************** grid_playerLoad ****************/
/* see grid.h for more detailed description */
grid_t*
grid_playerLoad(grid_t* mainGrid) {
    if (mainGrid == NULL) {
        log_e("Error: provided mainGrid is NULL");
        return NULL;
    }
    // allocate memory for new grid_t struct
    grid_t* player_grid = malloc(sizeof(grid_t));
    if (player_grid == NULL) {
        log_e("Error: could not allocate memory for grid");
        return NULL;
    }
    // initialize dimensions
    player_grid->nrow = mainGrid->nrow;
    player_grid->ncol = mainGrid->ncol;

    // allocate memory for gridArray
    player_grid->gridArray = malloc((player_grid->ncol + 1) * player_grid->nrow * sizeof(char));
    if (player_grid->gridArray == NULL) {
        log_e("Error: could not allocate memory for gridArray");
        free(player_grid);
        return NULL;
    } 
    // initialize all cells to hidden
    for (int r = 0; r < player_grid->nrow; r++) {
        for (int c = 0; c < player_grid->ncol; c++) {
            CELL(player_grid, r, c) = ' ';
        }
    }
    return player_grid;
}

void
grid_addGoldPile(grid_t* grid, int r, int c)
{
    if (grid == NULL) {
        flog_v(stderr, "Could not add gold pile to grid\n");
    }

    CELL(grid, r, c) = '*';
}

void
grid_removeGoldPile(grid_t* grid, int r, int c)
{
    if (grid == NULL) {
        flog_v(stderr, "Could not add gold pile to grid\n");
    }

    CELL(grid, r, c) = ' ';
}

/**************** grid_delete ****************/
/* see grid.h for more detailed description */
void
grid_delete(grid_t* grid)
{
    if (grid != NULL) {
        free(grid->gridArray);
        free(grid);
        log_v("Grid memory freed.");
    }
}

/**************** grid_validStart ****************/
/* see grid.h for more detailed description */
bool
grid_validStart(grid_t* grid, int r, int c)
{
    // check parameters
    if (r < 0 || r >= grid->nrow || c < 0 || c >= grid->ncol) {
        log_v("Invalid position access");
        return 0; 
    }
    // return boolean
    char cell = CELL(grid, r, c);
    return (cell == '.');
}  

/**************** grid_isWall ****************/
/* see grid.h for more detailed description */
bool 
grid_isWall(grid_t* grid, int r, int c)
{
    // check parameters
    if (r < 0 || r >= grid->nrow || c < 0 || c >= grid->ncol) {
        log_v("Invalid position access");
        return 0; 
    }
    // return boolean
    char cell = CELL(grid, r, c);
    return (cell == '+' || cell == '|' || cell == '-' || cell == ' ');
}

/**************** grid_isGold ****************/
/* see grid.h for more detailed description */
bool 
grid_isGold(grid_t* grid, int r, int c)
{
    // check parameters
    if (r < 0 || r >= grid->nrow || c < 0 || c >= grid->ncol) {
        log_v("Invalid position access");
        return 0; 
    }
    // return boolean
    char cell = CELL(grid, r, c);
    return (cell == '*');
}

/**************** grid_getCell ****************/
/* see grid.h for more detailed description */
char
grid_getCell(grid_t* grid, int r, int c)
{
    if (r < 0 || r >= grid->nrow || c < 0 || c >= grid->ncol) {
        log_v("Attempted out-of-bounds access");
        return ' ';  // Return empty space if out of bounds
    }
    return CELL(grid, r, c);
}

/**************** grid_getRows ****************/
/* see grid.h for more detailed description */
int 
grid_getRows(grid_t* grid)
{
    return grid->nrow;
}

/**************** grid_getCols ****************/
/* see grid.h for more detailed description */
int 
grid_getCols(grid_t* grid)
{
    return grid->ncol;
}

/**************** is_within_bounds ****************/
/* see grid.h for more detailed description */
bool 
is_within_bounds(grid_t* grid, int row, int col) 
{
    return row >= 0 && row < grid->nrow && col >= 0 && col < grid->ncol;
}

/************** grid_calculateVis ***************/
/* see grid.h for more detailed description */
void
grid_calculateVis(grid_t* mainGrid, grid_t* playerGrid, grid_t* originalGrid, int player_r, int player_c)
{
    for (int r = 0; r < mainGrid->nrow; r++) {
        for (int c = 0; c < mainGrid->ncol; c++) {
            // check line of sight
            if (line_of_sight(mainGrid, player_r, player_c, r, c)) {
                // update playerGrid
                CELL(playerGrid, r, c) = CELL(mainGrid, r, c);
            }
            // if not in current LOS but gold is here we have to hide from player  
            else if (CELL(mainGrid, r, c) == '*' && CELL(playerGrid, r, c) != ' ') {
                CELL(playerGrid, r, c) = '.';
            }
            // if not in current LOS but other player is here we have to hide from player
            else if (isupper(CELL(mainGrid, r, c)) && CELL(playerGrid, r, c) != ' ') {
                CELL(playerGrid, r, c) = CELL(originalGrid, r, c);
            }
        }
    }
}

/************** line_of_sight ***************/
/* see grid.h for more detailed description */
bool
line_of_sight(grid_t* mainGrid, int startRow, int startCol, int endRow, int endCol)
{
    // change in x and y
    int dx = abs(endCol - startCol);
    int dy = abs(endRow - startRow);

    // determine step directions
    int stepx = (startCol < endCol) ? 1 : -1;
    int stepy = (startRow < endRow) ? 1 : -1;

    // initialize current positions
    int x = startCol;
    int y = startRow;

    // handle same position
    if (dx == 0 && dy == 0) {
        return true;
    }

    // handle horizontal line
    if (dy == 0) {
        int x = startCol;
        while ((x + stepx) != endCol) {
            x += stepx;
            if (is_within_bounds(mainGrid, y, x)) {
                char cell = CELL(mainGrid, y, x);
                // only pass through gold, floor, or player
                if (!(cell == '.' || cell == '*' || isupper(cell))) {
                    return false;
                }
            } 
        }
        return true;
    }

    // handle vertical line
    if (dx == 0) {
        int y = startRow;
        while ((y + stepy) != endRow) {
            y += stepy;
            if (is_within_bounds(mainGrid, y, x)) {
                char cell = CELL(mainGrid, y, x);
                // only pass through gold, floor, or player
                if (!(cell == '.' || cell == '*' || isupper(cell))) {
                    return false;
                }
            }  
        }
        return true;
    }

    // continue until reaching the end position
    while ((x + stepx) != endCol || (y + stepy) != endRow) {
        // check row intersections
        if (y + stepy != endRow) {
            // increment
            y += stepy;

            // intersect calculation with slope
            double intersection_x;
            if (stepx == 1) {
                intersection_x = startCol + (double)abs(y - startRow) * dx / dy;
            }
            else {
                intersection_x = startCol + (double)abs(y - startRow) * -dx / dy;
            }

            // look to left and right of intersection
            int left_col = (int)floor(intersection_x);
            int right_col = (int)ceil(intersection_x);

            // direct grid hit
            if (left_col == right_col && is_within_bounds(mainGrid, y, left_col)) {
                char cell = CELL(mainGrid, y, left_col);
                // only pass through floor, gold, or corner
                if (!(cell == '.' || cell == '*' || isupper(cell))) {
                    return false;
                }
            } 
            else if (is_within_bounds(mainGrid, y, left_col) && is_within_bounds(mainGrid, y, right_col)) {
                char left_cell = CELL(mainGrid, y, left_col);
                char right_cell = CELL(mainGrid, y, right_col);
                // only pass if there is a floor or gold on either side
                if (!(left_cell == '.' || right_cell == '.' || left_cell == '*' || right_cell == '*' 
                        || isupper(left_cell) || isupper(right_cell))) {
                    return false;
                }
            }
        }
        // Check column intersections
        if (x + stepx != endCol) {
            // increment
            x += stepx;

            // calculate intersection with slope
            double intersection_y;
            if (stepy == 1) {
                intersection_y = startRow + (double)abs(x - startCol) * dy / dx;
            }
            else {
                intersection_y = startRow + (double)abs(x - startCol) * -dy / dx;
            }
            
            // look to top and bottom of intersect
            int top_row = (int)floor(intersection_y);
            int bottom_row = (int)ceil(intersection_y);

            // direct hit
            if (top_row == bottom_row && is_within_bounds(mainGrid, top_row, x)) {
                char cell = CELL(mainGrid, top_row, x);
                // only pass through floor, gold, or corner
                if (!(cell == '.' || cell == '*' || isupper(cell))) {
                    return false;
                }
            } 
            else if (is_within_bounds(mainGrid, top_row, x) && is_within_bounds(mainGrid, bottom_row, x)) {
                char top_cell = CELL(mainGrid, top_row, x);
                char bottom_cell = CELL(mainGrid, bottom_row, x);
                // only pass if floor or gold is on either side
                if (!(top_cell == '.' || bottom_cell == '.' || top_cell == '*' || bottom_cell == '*'
                        || isupper(top_cell) || isupper(bottom_cell))) {
                    return false;
                }
            }
        }
    }
    return true;
}

/************** grid_print ***************/
/* see grid.h for more detailed description */
void
grid_print(grid_t* grid) 
{
    if (grid == NULL) {
        log_e("Error: grid is NULL");
        return;
    }
    // loop through grid
    for (int r = 0; r < grid->nrow; r++) {
        for (int c = 0; c < grid->ncol; c++) {
            putchar(CELL(grid, r, c));
        }
        putchar('\n');
    }
}

void grid_toString(grid_t* grid, char* message) {
    if (grid == NULL || message == NULL) {
        log_e("Error: grid or message is NULL");
        return;
    }

    int maxSize = 10000;

    // Start with the intro
    snprintf(message, maxSize, "DISPLAY\n");
    int len = strlen(message);

    // Append rows of the grid
    for (int r = 0; r < grid->nrow && len < maxSize - 1; r++) {
        for (int c = 0; c < grid->ncol && len < maxSize - 1; c++) {
            message[len++] = CELL(grid, r, c);
        }
        // Add a newline character
        if (len < maxSize - 1) {
            message[len++] = '\n';
        }
    }

    // Null-terminate the string
    message[len] = '\0';
}
