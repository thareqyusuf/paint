/**
 * @file area_fill_algorithms.h
 * @brief Advanced area filling and flood fill algorithms
 */

#ifndef AREA_FILL_ALGORITHMS_H
#define AREA_FILL_ALGORITHMS_H

#include "coordinate_system.h"
#include "color_operations.h"
#include "coordinate_queue.h"

/**
 * @brief Queue-based flood fill algorithm for area filling
 * @param start_x Starting X coordinate
 * @param start_y Starting Y coordinate
 * @param new_fill_color Color to fill with
 * @param target_color Color to replace
 */
void flood_fill_enclosed_area(int start_x, int start_y, 
                             struct color_rgba new_fill_color,
                             struct color_rgba target_color);

/**
 * @brief Recursive flood fill implementation (alternative method)
 * @param x_coordinate Current X position
 * @param y_coordinate Current Y position
 * @param replacement_color New color to apply
 * @param original_color Original color being replaced
 */
void recursive_flood_fill(int x_coordinate, int y_coordinate, 
                         struct color_rgba replacement_color, 
                         struct color_rgba original_color);

/**
 * @brief Scanline-based raster fill for rectangular regions
 * @param minimum_y Top boundary
 * @param maximum_y Bottom boundary
 * @param minimum_x Left boundary
 * @param maximum_x Right boundary
 */
void raster_fill_rectangular_region(int minimum_y, int maximum_y, 
                                   int minimum_x, int maximum_x);

#endif

