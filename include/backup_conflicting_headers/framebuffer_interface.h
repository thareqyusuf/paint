/**
 * @file framebuffer_interface.h
 * @brief Low-level framebuffer hardware interface
 */

#ifndef FRAMEBUFFER_INTERFACE_H
#define FRAMEBUFFER_INTERFACE_H

#include "color_operations.h"
#include "coordinate_system.h"
#include <linux/fb.h>
#include <stdint.h>

// Global framebuffer state variables
extern int display_height;
extern int display_width;
extern char *framebuffer_memory_pointer;
extern long int total_screen_size;
extern struct fb_fix_screeninfo fixed_screen_info;
extern struct fb_var_screeninfo variable_screen_info;
extern int framebuffer_file_descriptor;
extern struct coordinate_point explosion_effect_points[11];
extern char pixel_availability_map[3840][2160];

/**
 * @brief Initialize pixel availability tracking system
 */
void initialize_pixel_availability_tracking(void);

/**
 * @brief Initialize framebuffer system and map video memory
 * @return 0 on success, negative error code on failure
 */
int initialize_framebuffer_system(void);

/**
 * @brief Set background color for entire framebuffer
 * @param background_color Color to fill background with
 */
void fill_background_color(struct color_rgba background_color);

/**
 * @brief Set pixel color with bounds checking and thickness support
 * @param thickness Square size for thick pixels
 * @param x_coordinate X position
 * @param y_coordinate Y position
 * @param pixel_color Color to set
 */
void set_pixel_with_thickness(int thickness, int x_coordinate, int y_coordinate, 
                             struct color_rgba pixel_color);

/**
 * @brief Get color value at specified pixel coordinates
 * @param x_coordinate X position
 * @param y_coordinate Y position
 * @return Color at specified position
 */
struct color_rgba get_pixel_color_at_position(int x_coordinate, int y_coordinate);

/**
 * @brief Clean shutdown of framebuffer system
 */
void terminate_framebuffer_system(void);

#endif

