#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include "color.h"
#include "point.h"

// Forward declarations
struct coordinate_point_queue;

// Core framebuffer functions  
int initScreen(void);
void terminate(void);
int initialize_framebuffer_system(void);
void terminate_framebuffer_system(void);

// Pixel and background operations
void fill_background_color(struct color_rgba background_color);
void set_pixel_with_thickness(int thickness, int x_coordinate, int y_coordinate, struct color_rgba pixel_color);
struct color_rgba get_pixel_color_at_position(int x_coordinate, int y_coordinate);

// Legacy compatibility functions
void setXY(int squareSize, int x, int y, struct color_rgba C);
struct color_rgba getXY(int x, int y);
void printBackground(struct color_rgba C);

// Color operations
struct color_rgba create_rgba_color(int red, int green, int blue);
int are_colors_similar(struct color_rgba first_color, struct color_rgba second_color);

// Basic geometry
void draw_line_between_points(struct coordinate_point start_point, 
                             struct coordinate_point end_point,
                             struct color_rgba line_color, 
                             int line_thickness);

void draw_circle_outline(int radius, struct coordinate_point center_point, 
                        int line_thickness, struct color_rgba circle_color);

// Fill operations
void flood_fill_enclosed_area(int start_x, int start_y,
                             struct color_rgba new_fill_color,
                             struct color_rgba target_color);

// Transformation operations
struct coordinate_point rotate_point_around_pivot(struct coordinate_point target_point,
                                                 struct coordinate_point pivot_point, 
                                                 float rotation_angle);

// Coordinate queue operations
void initialize_coordinate_queue(struct coordinate_point_queue* target_queue);
char is_coordinate_queue_empty(struct coordinate_point_queue* target_queue);
void enqueue_coordinate_point(struct coordinate_point_queue* target_queue, 
                             struct coordinate_point new_coordinate);
struct coordinate_point dequeue_coordinate_point(struct coordinate_point_queue* target_queue);

#endif // GRAPHICS_H
