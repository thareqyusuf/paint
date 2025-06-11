/**
 * @file legacy_compatibility.h
 * @brief Compatibility layer for legacy data structures
 */

#ifndef LEGACY_COMPATIBILITY_H
#define LEGACY_COMPATIBILITY_H

#include "color_operations.h"
#include "coordinate_system.h"

// Legacy type aliases for backward compatibility
typedef struct color_rgba Color;
typedef struct coordinate_point Point;

// Legacy constants
#define R red_component
#define G green_component  
#define B blue_component
#define x x_position
#define y y_position

#endif
