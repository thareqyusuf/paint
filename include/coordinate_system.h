/**
 * @file coordinate_system.h
 * @brief 2D coordinate system and point operations
 */

#ifndef COORDINATE_SYSTEM_H
#define COORDINATE_SYSTEM_H

/**
 * @brief 2D coordinate point structure
 */
typedef struct coordinate_point {
    int x_position;  ///< Horizontal coordinate
    int y_position;  ///< Vertical coordinate
} coordinate_point;

/**
 * @brief Create a coordinate point structure
 * @param x_coordinate X position
 * @param y_coordinate Y position
 * @return Initialized coordinate point
 */
struct coordinate_point create_coordinate_point(int x_coordinate, int y_coordinate);

#endif

