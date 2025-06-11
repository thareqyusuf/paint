/**
 * @file transformation_matrix.h
 * @brief 2D geometric transformation operations
 */

#ifndef TRANSFORMATION_MATRIX_H
#define TRANSFORMATION_MATRIX_H

#include "coordinate_system.h"

/**
 * @brief Rotate point around pivot using angle in degrees
 * @param target_point Point to rotate
 * @param pivot_point Center of rotation
 * @param rotation_angle_degrees Angle in degrees (positive = clockwise)
 * @return Transformed coordinate point
 */
struct coordinate_point rotate_point_around_pivot(struct coordinate_point target_point,
                                                 struct coordinate_point pivot_point, 
                                                 float rotation_angle_degrees);

/**
 * @brief Apply rotation transformation to array of points
 * @param pivot_point Center of rotation
 * @param point_array Array of points to transform
 * @param rotation_angle Rotation angle in degrees
 * @param array_length Number of points in array
 * @return Pointer to transformed point array
 */
struct coordinate_point* rotate_multiple_points(struct coordinate_point pivot_point, 
                                               struct coordinate_point* point_array, 
                                               double rotation_angle, 
                                               int array_length);

/**
 * @brief Apply scaling transformation to line endpoints
 * @param line_endpoints Array of 2 points representing line
 * @param x_scale_factor Horizontal scaling factor
 * @param y_scale_factor Vertical scaling factor
 */
void apply_scaling_to_line(struct coordinate_point* line_endpoints, 
                          double x_scale_factor, 
                          double y_scale_factor);

/**
 * @brief Apply translation transformation to line endpoints
 * @param line_endpoints Array of 2 points representing line
 * @param x_translation Horizontal offset
 * @param y_translation Vertical offset
 */
void apply_translation_to_line(struct coordinate_point* line_endpoints, 
                              int x_translation, 
                              int y_translation);

#endif

