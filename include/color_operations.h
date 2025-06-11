/**
 * @file color_operations.h
 * @brief Color manipulation and representation functions
 */

#ifndef COLOR_OPERATIONS_H
#define COLOR_OPERATIONS_H

#include <stdint.h>

/**
 * @brief RGBA color representation structure
 */
typedef struct color_rgba {
    int red_component;      ///< Red color intensity (0-255)
    int green_component;    ///< Green color intensity (0-255)
    int blue_component;     ///< Blue color intensity (0-255)
    int alpha_component;    ///< Alpha transparency (0-255)
} color_rgba;

/**
 * @brief Create an RGBA color structure
 * @param red Red component value (0-255)
 * @param green Green component value (0-255) 
 * @param blue Blue component value (0-255)
 * @return Initialized RGBA color structure
 */
struct color_rgba create_rgba_color(int red, int green, int blue);

/**
 * @brief Compare two colors for similarity within tolerance
 * @param first_color First color to compare
 * @param second_color Second color to compare
 * @return 1 if colors are similar, 0 otherwise
 */
int are_colors_similar(struct color_rgba first_color, struct color_rgba second_color);

#endif

