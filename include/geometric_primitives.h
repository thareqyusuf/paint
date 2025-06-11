/**
 * @file geometric_primitives.h
 * @brief Low-level geometric primitive rendering functions
 */

#ifndef GEOMETRIC_PRIMITIVES_H
#define GEOMETRIC_PRIMITIVES_H

#include "color_operations.h"
#include "coordinate_system.h"

/**
 * @brief Draw a line between two points using Bresenham algorithm
 * @param start_point Starting coordinate
 * @param end_point Ending coordinate
 * @param line_color Color for line rendering
 * @param line_thickness Thickness in pixels
 */
void draw_line_between_points(struct coordinate_point start_point, 
                             struct coordinate_point end_point,
                             struct color_rgba line_color, 
                             int line_thickness);

/**
 * @brief Draw connected line segments (polyline)
 * @param vertex_count Number of vertices
 * @param vertex_array Array of coordinate points
 * @param line_color Color for rendering
 * @param line_thickness Thickness in pixels
 */
void draw_connected_polyline(int vertex_count, struct coordinate_point *vertex_array, 
                            struct color_rgba line_color, int line_thickness);

/**
 * @brief Draw filled polygon
 * @param vertex_count Number of polygon vertices
 * @param vertex_array Array of coordinate points
 * @param fill_color Color for polygon fill
 * @param outline_thickness Outline thickness (0 for no outline)
 */
void draw_filled_polygon(int vertex_count, struct coordinate_point *vertex_array, 
                        struct color_rgba fill_color, int outline_thickness);

/**
 * @brief Draw circle using midpoint algorithm
 * @param radius Circle radius in pixels
 * @param center_point Center coordinate
 * @param line_thickness Outline thickness
 * @param circle_color Color for rendering
 */
void draw_circle_outline(int radius, struct coordinate_point center_point, 
                        int line_thickness, struct color_rgba circle_color);

/**
 * @brief Draw half-circle (semicircle)
 * @param radius Circle radius in pixels
 * @param center_point Center coordinate
 * @param line_thickness Outline thickness
 * @param circle_color Color for rendering
 */
void draw_semicircle_outline(int radius, struct coordinate_point center_point, 
                            int line_thickness, struct color_rgba circle_color);

/**
 * @brief Draw filled rectangle
 * @param top_left_x X coordinate of top-left corner
 * @param top_left_y Y coordinate of top-left corner
 * @param rectangle_width Width in pixels
 * @param rectangle_height Height in pixels
 * @param fill_color Color for rectangle fill
 */
void draw_filled_rectangle(int top_left_x, int top_left_y, 
                          int rectangle_width, int rectangle_height,
                          struct color_rgba fill_color);

#endif

