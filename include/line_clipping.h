/**
 * @file line_clipping.h
 * @brief Cohen-Sutherland line clipping algorithm implementation
 */

#ifndef LINE_CLIPPING_H
#define LINE_CLIPPING_H

#include "coordinate_system.h"
#include "color_operations.h"
#include <math.h>

// Clipping algorithm constants
#define COMPLETELY_INSIDE_VIEWPORT 1
#define COMPLETELY_OUTSIDE_VIEWPORT 2
#define INTERSECTS_VIEWPORT_BOUNDARY 3
#define REGION_CODE_BOTH_ZERO 4
#define REGION_CODE_BOTH_ONE 5
#define REGION_CODE_ONE_ZERO 6

/**
 * @brief Viewport clipping boundary definition
 */
typedef struct clipping_boundary {
    int left_boundary;
    int right_boundary;
    int top_boundary;
    int bottom_boundary;
} clipping_boundary;

/**
 * @brief Region code for Cohen-Sutherland algorithm
 */
typedef struct viewport_region_code {
    int left_region;
    int right_region;
    int top_region;
    int bottom_region;
} viewport_region_code;

/**
 * @brief Complete line analysis result for clipping operations
 */
typedef struct line_clipping_analysis {
    int overall_status_code;
    int left_boundary_intersection;
    int right_boundary_intersection;
    int top_boundary_intersection;
    int bottom_boundary_intersection;
    int start_point_inside_viewport;
    int end_point_inside_viewport;
    struct coordinate_point line_start_point;
    struct coordinate_point line_end_point;
    struct viewport_region_code start_point_region;
    struct viewport_region_code end_point_region;
} line_clipping_analysis;

/**
 * @brief Create clipping window with specified boundaries
 * @param left Left boundary coordinate
 * @param right Right boundary coordinate
 * @param top Top boundary coordinate
 * @param bottom Bottom boundary coordinate
 * @return Initialized clipping boundary structure
 */
struct clipping_boundary create_clipping_window(int left, int right, int top, int bottom);

/**
 * @brief Initialize empty region code structure
 * @return Zero-initialized region code
 */
struct viewport_region_code initialize_empty_region_code(void);

/**
 * @brief Compute region code for point relative to clipping window
 * @param test_point Point to analyze
 * @param clipping_window Viewport boundaries
 * @return Region code indicating point position
 */
struct viewport_region_code compute_point_region_code(struct coordinate_point test_point, 
                                                     struct clipping_boundary clipping_window);

/**
 * @brief Analyze line segment for clipping requirements
 * @param line_start Starting point of line
 * @param line_end Ending point of line
 * @param clipping_window Viewport boundaries
 * @return Complete analysis of line clipping requirements
 */
struct line_clipping_analysis analyze_line_for_clipping(struct coordinate_point line_start, 
                                                       struct coordinate_point line_end, 
                                                       struct clipping_boundary clipping_window);

/**
 * @brief Perform Cohen-Sutherland line clipping
 * @param clipping_analysis Pre-computed line analysis
 * @param clipping_window Viewport boundaries
 * @param clipped_output Output array for clipped line endpoints
 */
void perform_line_clipping_operation(struct line_clipping_analysis clipping_analysis, 
                                    struct clipping_boundary clipping_window, 
                                    struct coordinate_point* clipped_output);

#endif

