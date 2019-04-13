#ifndef __DEF_H__
#define __DEF_H__

#include <vector>
#include <cmath>

// For epsilon difference.
#define EPSILON 1e-9f

// Are two floats within epsilon difference.
#define IS_CLOSE(x,y) ((float)abs((x)-(y)) < EPSILON)

// Table z-axis thickness.
#define TABLE_HEIGHT 0.15f

// Block edge length.
#define BLOCK_EDGE 0.5f
#define BLOCK_HALF_EDGE 0.25f

// One hop distance on grid.
#define GRID_SIZE 0.5f

// Starting point for my block.
#define GRID_START_X 2.25f
#define GRID_START_Y -2.25f

// Positive/Negative end of grid.
#define POS_GRID_END 2.5f
#define NEG_GRID_END -2.5f

// Number of blocks scattered on the grid.
#define NUM_BLOCKS 7

//float grid_centers[] = {
std::vector<float> grid_centers = {
    GRID_START_Y,
    GRID_START_Y + 1 * GRID_SIZE,
    GRID_START_Y + 2 * GRID_SIZE,
    GRID_START_Y + 3 * GRID_SIZE,
    GRID_START_Y + 4 * GRID_SIZE,
    GRID_START_Y + 5 * GRID_SIZE,
    GRID_START_Y + 6 * GRID_SIZE,
    GRID_START_Y + 7 * GRID_SIZE,
    GRID_START_Y + 8 * GRID_SIZE,
    GRID_START_Y + 9 * GRID_SIZE
};

#endif
