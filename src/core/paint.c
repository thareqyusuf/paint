/**
 * @file paint.c
 * @brief Main framebuffer graphics application with real-time drawing capabilities
 * @author Your Name
 * 
 * This application demonstrates low-level graphics programming by directly
 * interfacing with the Linux framebuffer device. It provides real-time
 * drawing capabilities with geometric primitives, transformations, and
 * interactive controls.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

#include "../../include/graphics_engine.h"
#include "../../include/geometric_primitives.h"
#include "../../include/transformation_matrix.h"
#include "../../include/area_fill_algorithms.h"
#include "../../include/line_clipping.h"
#include "../../include/keyboard_input.h"

/**
 * @brief Structure representing a complete polygon with color information
 */
typedef struct geometric_polygon {
    struct color_rgba polygon_color;
    struct coordinate_point vertices[MAX_POLYGON_VERTICES];
    int active_vertex_count;
} geometric_polygon;

// Global application state variables
double viewport_center_x = 500;
double viewport_center_y = 250;
double current_scale_factor = 1.0;
int current_rotation_degrees = 0;
int text_rotation_degrees = 0;
int active_vertex_count = 0;
int colored_point_count = 0;
double scaling_factor = 1.0;
unsigned char current_input_state = 0;
unsigned char fill_mode_active = 0;
unsigned char draw_square_mode = 0;
unsigned char draw_rectangle_mode = 0;
unsigned char draw_triangle_mode = 0;

pthread_t keyboard_input_thread;

// Polygon management arrays
int total_polygon_count = 0;
struct coordinate_point* vertex_buffer;
geometric_polygon rendered_polygons[MAX_RENDERED_POLYGONS];
struct color_rgba available_colors[TOTAL_COLOR_COUNT];
int currently_selected_color;
struct coordinate_point colored_fill_points[MAX_FILL_POINTS];
struct color_rgba selected_fill_colors[MAX_FILL_POINTS];

/**
 * @brief Utility function to check if a value is within specified bounds
 * @param lower_bound Minimum acceptable value
 * @param upper_bound Maximum acceptable value  
 * @param test_value Value to test
 * @return 1 if within bounds, 0 otherwise
 */
unsigned char is_value_within_bounds(int lower_bound, int upper_bound, int test_value) {
    return test_value > lower_bound && test_value < upper_bound;
}

/**
 * @brief Validates that all points in a line are within reasonable coordinate bounds
 * @param point_count Number of points in the line
 * @param coordinate_array Array of coordinate points
 * @return 1 if all points are valid, 0 otherwise
 */
unsigned char validate_line_coordinates(int point_count, struct coordinate_point* coordinate_array) {
    for (size_t i = 0; i < point_count; i++) {
        // Validate coordinates are within reasonable bounds for graphics operations
        if (!is_value_within_bounds(-50000, 100000, coordinate_array[i].x_position) || 
            !is_value_within_bounds(-50000, 100000, coordinate_array[i].y_position)) {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Load and render geometric data from external file
 * @param filename Path to the data file
 * @param is_polygon_data True for polygon rendering, false for polyline
 * @param render_color Color to use for rendering
 */
void load_and_render_from_file(const char* filename, unsigned char is_polygon_data, struct color_rgba render_color) {
    FILE* input_file = fopen(filename, "r");
    if (!input_file) {
        fprintf(stderr, "Error: Cannot open geometry file %s\n", filename);
        return;
    }

    // Set up clipping window for viewport management
    struct clipping_boundary viewport_boundary = create_clipping_window(
        current_scale_factor * (100 + viewport_center_x),
        current_scale_factor * (200 + viewport_center_x),
        current_scale_factor * (200 + viewport_center_y),
        current_scale_factor * (100 + viewport_center_y)
    );

    // Calculate transformation parameters for viewport mapping
    double x_scaling_factor = (1000 - 500) / ((current_scale_factor * (200 + viewport_center_x)) - 
                                             (current_scale_factor * (100 + viewport_center_x)));
    double y_scaling_factor = (550 - 50) / (current_scale_factor * ((200 + viewport_center_y)) - 
                                           current_scale_factor * ((100 + viewport_center_y)));
    double x_translation = (((current_scale_factor * (200 + viewport_center_x)) * 500) - 
                           ((current_scale_factor * (100 + viewport_center_x)) * 1000)) / 
                          ((current_scale_factor * (200 + viewport_center_x)) - 
                           (current_scale_factor * (100 + viewport_center_x)));
    double y_translation = (((current_scale_factor * (200 + viewport_center_y)) * 50) - 
                           ((current_scale_factor * (100 + viewport_center_y)) * 550)) / 
                          ((current_scale_factor * (200 + viewport_center_y)) - 
                           (current_scale_factor * (100 + viewport_center_y)));

    // Process geometric data from file
    while (!feof(input_file)) {
        int point_count;
        if (fscanf(input_file, "%d", &point_count) != 1) break;
        
        struct coordinate_point* geometry_points = malloc(point_count * sizeof(struct coordinate_point));
        if (!geometry_points) {
            fprintf(stderr, "Error: Failed to allocate memory for geometry points\n");
            break;
        }

        // Read coordinate data
        for (int i = 0; i < point_count; i++) {
            if (fscanf(input_file, "%d %d", &geometry_points[i].x_position, 
                      &geometry_points[i].y_position) != 2) {
                break;
            }
        }

        // Render based on data type
        if (is_polygon_data) {
            draw_filled_polygon(point_count, geometry_points, render_color, 1);
        } else {
            draw_connected_polyline(point_count, geometry_points, render_color, 1);
        }

        free(geometry_points);
    }
    fclose(input_file);
}

/**
 * @brief Check if a point is inside a given polygon using bounding box test
 * @param test_x X coordinate to test
 * @param test_y Y coordinate to test  
 * @param target_polygon Polygon to test against
 * @return 1 if point is inside, 0 otherwise
 */
int is_point_inside_polygon(int test_x, int test_y, geometric_polygon target_polygon) {
    int max_y = 0, max_x = 0;
    int min_y = 99999, min_x = 99999;
    
    // Calculate bounding box
    for (int i = 0; i < target_polygon.active_vertex_count; i++) {
        if (max_x < target_polygon.vertices[i].x_position) {
            max_x = target_polygon.vertices[i].x_position;
        }
        if (min_x > target_polygon.vertices[i].x_position) {
            min_x = target_polygon.vertices[i].x_position;
        }
        if (max_y < target_polygon.vertices[i].y_position) {
            max_y = target_polygon.vertices[i].y_position;
        }
        if (min_y > target_polygon.vertices[i].y_position) {
            min_y = target_polygon.vertices[i].y_position;
        }
    }

    // Perform bounding box test
    return (max_x >= test_x && min_x <= test_x && min_y <= test_y && max_y >= test_y);
}

/**
 * @brief Main screen refresh function - redraws entire framebuffer content
 * 
 * This function demonstrates real-time graphics rendering by:
 * 1. Clearing the framebuffer background
 * 2. Rendering UI elements and viewport boundaries  
 * 3. Drawing all active geometric objects
 * 4. Handling interactive drawing modes
 * 5. Processing fill operations
 */
void refresh_graphics_display(void) {
    // Clear framebuffer with black background
    fill_background_color(create_rgba_color(0, 0, 0));
    
    // Allocate clipping window coordinates
    struct coordinate_point* viewport_boundary;
    struct coordinate_point viewport_center;
    
    viewport_boundary = (struct coordinate_point*) malloc(4 * sizeof(struct coordinate_point));
    viewport_boundary[0] = create_coordinate_point(current_scale_factor * (0 + viewport_center_x),
                                                  current_scale_factor * (0 + viewport_center_y));
    viewport_boundary[1] = create_coordinate_point(current_scale_factor * (0 + viewport_center_x),
                                                  current_scale_factor * (1 + viewport_center_y));
    viewport_boundary[2] = create_coordinate_point(current_scale_factor * (1 + viewport_center_x),
                                                  current_scale_factor * (1 + viewport_center_y));
    viewport_boundary[3] = create_coordinate_point(current_scale_factor * (1 + viewport_center_x),
                                                  current_scale_factor * (0 + viewport_center_y));
    viewport_center = create_coordinate_point(current_scale_factor * (0 + viewport_center_x),
                                             current_scale_factor * (150 + viewport_center_y));

    // Draw viewport boundary
    draw_filled_polygon(4, viewport_boundary, available_colors[currently_selected_color], 4);

    // Define main viewing window
    struct coordinate_point viewing_window[4];
    viewing_window[0] = create_coordinate_point(50, 50);
    viewing_window[1] = create_coordinate_point(50, 550);
    viewing_window[2] = create_coordinate_point(1000, 550);
    viewing_window[3] = create_coordinate_point(1000, 50);

    // Draw triangle mode indicator
    struct coordinate_point triangle_mode_indicator[4];
    triangle_mode_indicator[0] = create_coordinate_point(120, 60);
    triangle_mode_indicator[1] = create_coordinate_point(120, 110);
    triangle_mode_indicator[2] = create_coordinate_point(170, 110);
    triangle_mode_indicator[3] = create_coordinate_point(170, 60);
    
    if (draw_triangle_mode) {
        draw_filled_polygon(4, triangle_mode_indicator, create_rgba_color(0, 100, 180), 1);
    } else {
        draw_filled_polygon(4, triangle_mode_indicator, create_rgba_color(0, 255, 180), 1);
    }

    // Draw rectangle mode indicator
    struct coordinate_point rectangle_mode_indicator[4];
    rectangle_mode_indicator[0] = create_coordinate_point(180, 60);
    rectangle_mode_indicator[1] = create_coordinate_point(180, 110);
    rectangle_mode_indicator[2] = create_coordinate_point(230, 110);
    rectangle_mode_indicator[3] = create_coordinate_point(230, 60);
    
    if (draw_rectangle_mode) {
        draw_filled_polygon(4, rectangle_mode_indicator, create_rgba_color(0, 100, 180), 1);
    } else {
        draw_filled_polygon(4, rectangle_mode_indicator, create_rgba_color(0, 255, 180), 1);
    }

    // Render all active polygons
    for (int i = 0; i < MAX_RENDERED_POLYGONS; i++) {
        draw_filled_polygon(rendered_polygons[i].active_vertex_count, 
                           rendered_polygons[i].vertices, 
                           rendered_polygons[i].polygon_color, 1);
        
        // Apply flood fill to colored points (commented out - replace with proper fill function)
        // flood_fill_area(colored_fill_points[i].x_position, colored_fill_points[i].y_position, 
        //                selected_fill_colors[i], create_rgba_color(0, 0, 0));
    }

    // Handle fill mode activation
    if (fill_mode_active) {
        colored_fill_points[colored_point_count].x_position = viewport_center_x;
        colored_fill_points[colored_point_count].y_position = viewport_center_y;
        selected_fill_colors[colored_point_count] = available_colors[currently_selected_color];
        colored_point_count++;
        fill_mode_active = !fill_mode_active;
        refresh_graphics_display(); // Recursive call for immediate update
    }

    // Handle rectangle drawing mode
    if (draw_rectangle_mode) {
        int user_command = get_next_keyboard_input();

        if (user_command == SPACE_KEY_CODE) {
            if (active_vertex_count == 0) {
                vertex_buffer[active_vertex_count] = create_coordinate_point(viewport_center_x, viewport_center_y);
                active_vertex_count++;
            } else {
                // Complete rectangle by adding remaining vertices
                vertex_buffer[active_vertex_count] = create_coordinate_point(vertex_buffer[0].x_position, viewport_center_y);
                active_vertex_count++;
                vertex_buffer[active_vertex_count] = create_coordinate_point(viewport_center_x, viewport_center_y);
                active_vertex_count++;
                vertex_buffer[active_vertex_count] = create_coordinate_point(viewport_center_x, vertex_buffer[0].y_position);
                active_vertex_count++;

                if (active_vertex_count == 4) {
                    // Store completed rectangle
                    for (int i = 0; i < active_vertex_count; i++) {
                        rendered_polygons[total_polygon_count].vertices[i] = vertex_buffer[i];
                        rendered_polygons[total_polygon_count].active_vertex_count = active_vertex_count;
                        rendered_polygons[total_polygon_count].polygon_color = available_colors[currently_selected_color];
                    }
                    
                    // Reset vertex buffer
                    for (int i = 0; i < active_vertex_count; i++) {
                        vertex_buffer[i].x_position = 0;
                        vertex_buffer[i].y_position = 0;
                    }
                    active_vertex_count = 0;
                    total_polygon_count++;
                    draw_rectangle_mode = !draw_rectangle_mode;
                    refresh_graphics_display();
                }
            }
        }
    }

    // Handle triangle drawing mode
    if (draw_triangle_mode) {
        int user_command = get_next_keyboard_input();

        if (user_command == SPACE_KEY_CODE) {
            vertex_buffer[active_vertex_count] = create_coordinate_point(viewport_center_x, viewport_center_y);
            active_vertex_count++;
            
            if (active_vertex_count == 3) {
                // Store completed triangle
                for (int i = 0; i < active_vertex_count; i++) {
                    rendered_polygons[total_polygon_count].vertices[i] = vertex_buffer[i];
                    rendered_polygons[total_polygon_count].active_vertex_count = active_vertex_count;
                    rendered_polygons[total_polygon_count].polygon_color = available_colors[currently_selected_color];
                }
                
                // Reset vertex buffer
                for (int i = 0; i < active_vertex_count; i++) {
                    vertex_buffer[i].x_position = 0;
                    vertex_buffer[i].y_position = 0;
                }
                active_vertex_count = 0;
                total_polygon_count++;
                draw_triangle_mode = !draw_triangle_mode;
                refresh_graphics_display();
            }
        }
    }
    
    free(viewport_boundary);
}

/**
 * @brief Zoom out operation - reduces scale of all rendered polygons
 */
void apply_zoom_out_transformation(void) {
    for (int i = 0; i < total_polygon_count; i++) {
        for (int j = 0; j < rendered_polygons[i].active_vertex_count; j++) {
            rendered_polygons[i].vertices[j].x_position = rendered_polygons[i].vertices[j].x_position * 9 / 10;
            rendered_polygons[i].vertices[j].y_position = rendered_polygons[i].vertices[j].y_position * 9 / 10;
        }
    }
}

/**
 * @brief Zoom in operation - increases scale of all rendered polygons
 */
void apply_zoom_in_transformation(void) {
    for (int i = 0; i < total_polygon_count; i++) {
        for (int j = 0; j < rendered_polygons[i].active_vertex_count; j++) {
            rendered_polygons[i].vertices[j].x_position = rendered_polygons[i].vertices[j].x_position * 10 / 9;
            rendered_polygons[i].vertices[j].y_position = rendered_polygons[i].vertices[j].y_position * 10 / 9;
        }
    }
}

/**
 * @brief Rotate polygon counter-clockwise around cursor position
 */
void rotate_polygon_counter_clockwise(void) {
    for (int i = 0; i < total_polygon_count; i++) {
        if (is_point_inside_polygon(viewport_center_x, viewport_center_y, rendered_polygons[i])) {
            for (int j = 0; j < rendered_polygons[i].active_vertex_count; j++) {
                float sine_value = sin(-10 * 3.14159 / 180.0);
                float cosine_value = cos(-10 * 3.14159 / 180.0);
                
                // Translate to origin
                rendered_polygons[i].vertices[j].x_position -= viewport_center_x;
                rendered_polygons[i].vertices[j].y_position -= viewport_center_y;

                // Apply rotation transformation
                int new_x = (int)(rendered_polygons[i].vertices[j].x_position * cosine_value - 
                                 rendered_polygons[i].vertices[j].y_position * sine_value);
                int new_y = (int)(rendered_polygons[i].vertices[j].x_position * sine_value + 
                                 rendered_polygons[i].vertices[j].y_position * cosine_value);

                // Translate back to original position
                rendered_polygons[i].vertices[j].x_position = new_x + viewport_center_x;
                rendered_polygons[i].vertices[j].y_position = new_y + viewport_center_y;
            }
        }
    }
}

/**
 * @brief Rotate polygon clockwise around cursor position
 */
void rotate_polygon_clockwise(void) {
    for (int i = 0; i < total_polygon_count; i++) {
        if (is_point_inside_polygon(viewport_center_x, viewport_center_y, rendered_polygons[i])) {
            for (int j = 0; j < rendered_polygons[i].active_vertex_count; j++) {
                float sine_value = sin(10 * 3.14159 / 180.0);
                float cosine_value = cos(10 * 3.14159 / 180.0);
                
                // Translate to origin
                rendered_polygons[i].vertices[j].x_position -= viewport_center_x;
                rendered_polygons[i].vertices[j].y_position -= viewport_center_y;

                // Apply rotation transformation  
                int new_x = (int)(rendered_polygons[i].vertices[j].x_position * cosine_value - 
                                 rendered_polygons[i].vertices[j].y_position * sine_value);
                int new_y = (int)(rendered_polygons[i].vertices[j].x_position * sine_value + 
                                 rendered_polygons[i].vertices[j].y_position * cosine_value);

                // Translate back to original position
                rendered_polygons[i].vertices[j].x_position = new_x + viewport_center_x;
                rendered_polygons[i].vertices[j].y_position = new_y + viewport_center_y;
            }
        }
    }
}

/**
 * @brief Scale down polygons under cursor position
 */
void scale_down_polygon_at_cursor(void) {
    for (int i = 0; i < total_polygon_count; i++) {
        if (is_point_inside_polygon(viewport_center_x, viewport_center_y, rendered_polygons[i])) {
            for (int j = 0; j < rendered_polygons[i].active_vertex_count; j++) {
                rendered_polygons[i].vertices[j].x_position = rendered_polygons[i].vertices[j].x_position * 9 / 10;
                rendered_polygons[i].vertices[j].y_position = rendered_polygons[i].vertices[j].y_position * 9 / 10;
            }
        }
    }
}

/**
 * @brief Scale up polygons under cursor position
 */
void scale_up_polygon_at_cursor(void) {
    for (int i = 0; i < total_polygon_count; i++) {
        if (is_point_inside_polygon(viewport_center_x, viewport_center_y, rendered_polygons[i])) {
            for (int j = 0; j < rendered_polygons[i].active_vertex_count; j++) {
                rendered_polygons[i].vertices[j].x_position = rendered_polygons[i].vertices[j].x_position * 10 / 9;
                rendered_polygons[i].vertices[j].y_position = rendered_polygons[i].vertices[j].y_position * 10 / 9;
            }
        }
    }
}

/**
 * @brief Save current drawing state to file
 */
void save_drawing_to_file(void) {
    FILE *output_file = fopen("saved_drawing.txt", "w");
    if (output_file == NULL) {
        printf("Error: Cannot create save file\n");
        return;
    }

    // Write polygon data to file
    for (int i = 0; i < total_polygon_count; i++) {
        fprintf(output_file, "%d ", rendered_polygons[i].active_vertex_count);
        for (int j = 0; j < rendered_polygons[i].active_vertex_count; j++) {
            fprintf(output_file, "%d %d ", 
                   rendered_polygons[i].vertices[j].x_position,
                   rendered_polygons[i].vertices[j].y_position);
        }
        fprintf(output_file, "%d %d %d\n", 
               rendered_polygons[i].polygon_color.red_component,
               rendered_polygons[i].polygon_color.green_component,
               rendered_polygons[i].polygon_color.blue_component);
    }

    fclose(output_file);
    printf("Drawing saved successfully\n");
}

/**
 * @brief Keyboard input listener thread function
 * @param thread_parameter Unused parameter for pthread compatibility
 * @return NULL
 */
void *process_keyboard_input_continuously(void *thread_parameter) {
    int user_command = ' ';
    while (1) {
        user_command = get_next_keyboard_input();
        
        // Movement controls
        if (user_command == ARROW_LEFT_KEY) { 
            viewport_center_x -= 20; 
            refresh_graphics_display();
        }
        else if (user_command == ARROW_RIGHT_KEY) {
            viewport_center_x += 20;
            refresh_graphics_display();
        }
        else if (user_command == ARROW_UP_KEY) {
            viewport_center_y += 20; 
            refresh_graphics_display();
        }
        else if (user_command == ARROW_DOWN_KEY) {
            viewport_center_y -= 20; 
            refresh_graphics_display();
        }
        
        // Zoom controls
        else if (user_command == ZOOM_IN_KEY) {
            current_scale_factor -= 0.1; 
            refresh_graphics_display();
        }
        else if (user_command == ZOOM_OUT_KEY) {
            current_scale_factor += 0.1; 
            refresh_graphics_display();
        }
        
        // Drawing mode controls
        else if (user_command == FILL_MODE_KEY) {
            fill_mode_active = !fill_mode_active; 
            refresh_graphics_display();
        }
        else if (user_command == TRIANGLE_MODE_KEY) {
            draw_triangle_mode = !draw_triangle_mode; 
            refresh_graphics_display();
        }
        else if (user_command == RECTANGLE_MODE_KEY) {
            draw_rectangle_mode = !draw_rectangle_mode; 
            refresh_graphics_display();
        }
        
        // Rotation controls
        else if (user_command == GLOBAL_ROTATE_KEY) {
            current_rotation_degrees = (current_rotation_degrees + 10) % 360;
            refresh_graphics_display();
        }
        
        // Color selection
        else if (user_command == PREVIOUS_COLOR_KEY) {
            if (currently_selected_color == 0) {
                currently_selected_color = 3;
            } else {
                currently_selected_color--;
            }
            refresh_graphics_display();
        }
        else if (user_command == NEXT_COLOR_KEY) {
            if (currently_selected_color == 3) {
                currently_selected_color = 0;
            } else {
                currently_selected_color++;
            }
            refresh_graphics_display();
        }
        
        // Global transformations
        else if (user_command == GLOBAL_ZOOM_OUT_KEY) {
            apply_zoom_out_transformation(); 
            refresh_graphics_display();
        }
        else if (user_command == GLOBAL_ZOOM_IN_KEY) {
            apply_zoom_in_transformation(); 
            refresh_graphics_display();
        }
        else if (user_command == ROTATE_LEFT_KEY) {
            rotate_polygon_counter_clockwise(); 
            refresh_graphics_display();
        }
        else if (user_command == ROTATE_RIGHT_KEY) {
            rotate_polygon_clockwise(); 
            refresh_graphics_display();
        }
        else if (user_command == SCALE_DOWN_KEY) {
            scale_down_polygon_at_cursor(); 
            refresh_graphics_display();
        }
        else if (user_command == SCALE_UP_KEY) {
            scale_up_polygon_at_cursor(); 
            refresh_graphics_display();
        }
        else if (user_command == SAVE_DRAWING_KEY) {
            save_drawing_to_file(); 
            refresh_graphics_display();
        }
    }
    return NULL;
}

/**
 * @brief Infinite loop to prevent program termination
 */
void maintain_program_execution(void) {
    while(1) {}
}

/**
 * @brief Initialize color palette with predefined values
 */
void initialize_color_palette(void) {
    available_colors[0] = create_rgba_color(255, 255, 255); // White
    available_colors[1] = create_rgba_color(255, 0, 0);     // Red  
    available_colors[2] = create_rgba_color(0, 255, 0);     // Green
    available_colors[3] = create_rgba_color(0, 0, 255);     // Blue
}

/**
 * @brief Main application entry point
 * @return Exit status code
 */
int main(void) {
    // Initialize vertex buffer
    vertex_buffer = malloc(MAX_POLYGON_VERTICES * sizeof(struct coordinate_point));
    if (!vertex_buffer) {
        fprintf(stderr, "Error: Failed to allocate vertex buffer\n");
        return 1;
    }
    
    // Initialize graphics systems
    initialize_framebuffer_system();
    initialize_color_palette();
    fill_background_color(create_rgba_color(0, 0, 0));
    refresh_graphics_display();
    
    // Start keyboard input processing thread
    pthread_create(&keyboard_input_thread, NULL, process_keyboard_input_continuously, NULL);
    
    // Maintain program execution
    maintain_program_execution();
    
    // Cleanup (unreachable in current implementation)
    terminate_framebuffer_system();
    free(vertex_buffer);
    return 0;
}
