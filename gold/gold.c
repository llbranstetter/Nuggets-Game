
/*************
 * gold.c 
 * see gold.h for more explanation
 * 
 * Kellen Seeley
 * Group Big D Nuggets
 * Nuggets final project gold module
 */
#include <stdlib.h>
#include <stdio.h>
#include "gold.h"


/********** function prototypes **************/
gold_t* gold_new(int value, int x, int y, int ncols);
void gold_view(gold_t* gold);
int gold_collect(gold_t* gold);
void gold_delete(gold_t* gold);

/************ global functions *************/

/**************** gold_new *****************/
/**
 * creates a new gold object at specified (x, y) position
 */
gold_t* gold_new(int value, int x, int y, int ncols) {

    gold_t* gold = malloc(sizeof(gold_t)); 

    if (gold == NULL) return NULL;

    gold->isFound = false;
    gold->isVisible = false;
    gold->value = value;
    gold->placement = y * (ncols + 1) + x;  // convert (x, y) to a single placement index

    return gold;
}


/**************** gold_view *****************/
/**
 * sets the gold as visible to the player
 */
void gold_view(gold_t* gold) {
    if (gold != NULL) {
        gold->isVisible = true;
    }
}

/**************** gold_collect *****************/
/**
 * collects the gold, marks it as found, and returns its value
 */
int gold_collect(gold_t* gold) {
    if (gold != NULL && !gold->isFound) {
        gold->isFound = true;
        return gold->value;
    }
    return 0;
}

/**************** gold_delete *****************/
/**
 * free memory allocated for the gold object
 */
void gold_delete(gold_t* gold) {
    if (gold != NULL) {
        free(gold);
    }
}