/*************
 * gold.h 
 * .h file for gold.c 
 * 
 * Kellen Seeley
 * Group Big D Nuggets
 * Nuggets final project gold module
 */
#ifndef GOLD_H
#define GOLD_H

#include <stdbool.h>


/************ global structure **************/
/* gold struct to represent each gold pile in the game */
typedef struct gold {
    bool isFound;    // indicates if the gold has been collected
    int placement;   // location of the gold on the grid
    bool isVisible;  // indicates if the player can see the gold
    int value;       // value of the gold pile
} gold_t;

/**************** gold_new *****************/
/**
 * creates a new gold object with specified value and placement
 * initializes `isFound` and `isVisible` to false
 * 
 * @param value the value of the gold pile
 * @param x the x-coordinate on the grid
 * @param y the y-coordinate on the grid
 * @param ncols the number of columns in the grid
 * @return a pointer to the newly created gold_t, or NULL on failure
 */
gold_t* gold_new(int value, int x, int y, int ncols);

/**************** gold_view *****************/
/**
 * sets the visibility of the gold object to true for when in view
 *
 * @param gold the gold object whose visibility will be set
 */
void gold_view(gold_t* gold);

/**************** gold_collect *****************/
/**
 * marks the gold object as found, removes it from visibility,
 * and returns its value.
 *
 * @param gold the gold object to be collected
 * @return the value of the gold collected, or 0 if already collected
 */
int gold_collect(gold_t* gold);

/**************** gold_delete *****************/
/**
 * frees the memory allocated for the gold object
 *
 * @param gold the gold object to delete
 */
void gold_delete(gold_t* gold);

#endif 
