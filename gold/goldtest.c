#include <stdio.h>
#include <stdlib.h>
#include "gold.h"

int main() {
    printf("Unit test for gold module starting...\n");

    // test gold_new
    gold_t* gold = gold_new(50, 2, 3, 10); // (value, x, y, cols)
    if (gold == NULL) {
        printf("FAILED: gold_new returned NULL\n");
        return 1;
    }
    if (gold->value != 50) {
        printf("FAILED: gold_new did not set value correctly\n");
        return 1;
    }
    if (gold->placement != 32) { // placement = y * ncols + x = 3 * 10 + 2 = 32
        printf("FAILED: gold_new did not calculate placement correctly\n");
        return 1;
    }
    if (gold->isFound != false) {
        printf("FAILED: gold_new did not initialize isFound correctly\n");
        return 1;
    }
    if (gold->isVisible != false) {
        printf("FAILED: gold_new did not initialize isVisible correctly\n");
        return 1;
    }
    printf("PASSED: gold_new test\n");

    // test gold_view
    gold_view(gold);
    if (gold->isVisible != true) {
        printf("FAILED: gold_view did not set isVisible to true\n");
        return 1;
    }
    printf("PASSED: gold_view test\n");

    // test gold_collect
    int value = gold_collect(gold);
    if (value != 50) {
        printf("FAILED: gold_collect did not return correct value\n");
        return 1;
    }
    if (gold->isFound != true) {
        printf("FAILED: gold_collect did not set isFound to true\n");
        return 1;
    }

    value = gold_collect(gold); // trying to collect again should return 0
    if (value != 0) {
        printf("FAILED: gold_collect did not return 0 for already collected gold\n");
        return 1;
    }
    printf("PASSED: gold_collect test\n");

    // test gold_delete
    gold_delete(gold);
    printf("PASSED: gold_delete test\n");

    printf("All tests PASSED for gold module!\n");
    return 0;
}