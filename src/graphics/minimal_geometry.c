#include "framebuffer.h"

// Minimal implementation of missing geometry functions
void draw_line_between_points(struct coordinate_point start_point, 
                             struct coordinate_point end_point,
                             struct color_rgba line_color, 
                             int line_thickness) {
    // Simple line drawing using existing setXY function
    int dx = abs(end_point.x_coordinate - start_point.x_coordinate);
    int dy = abs(end_point.y_coordinate - start_point.y_coordinate);
    int x = start_point.x_coordinate;
    int y = start_point.y_coordinate;
    int x_inc = (start_point.x_coordinate < end_point.x_coordinate) ? 1 : -1;
    int y_inc = (start_point.y_coordinate < end_point.y_coordinate) ? 1 : -1;
    int error = dx - dy;
    
    while (x != end_point.x_coordinate || y != end_point.y_coordinate) {
        setXY(line_thickness, x, y, line_color);
        
        int error2 = 2 * error;
        if (error2 > -dy) {
            error -= dy;
            x += x_inc;
        }
        if (error2 < dx) {
            error += dx;
            y += y_inc;
        }
    }
}

void draw_circle_outline(int radius, struct coordinate_point center_point, 
                        int line_thickness, struct color_rgba circle_color) {
    // Simple circle drawing using existing setXY function
    int x = 0;
    int y = radius;
    int decision = 1 - radius;
    
    while (x <= y) {
        setXY(line_thickness, center_point.x_coordinate + x, center_point.y_coordinate + y, circle_color);
        setXY(line_thickness, center_point.x_coordinate - x, center_point.y_coordinate + y, circle_color);
        setXY(line_thickness, center_point.x_coordinate + x, center_point.y_coordinate - y, circle_color);
        setXY(line_thickness, center_point.x_coordinate - x, center_point.y_coordinate - y, circle_color);
        setXY(line_thickness, center_point.x_coordinate + y, center_point.y_coordinate + x, circle_color);
        setXY(line_thickness, center_point.x_coordinate - y, center_point.y_coordinate + x, circle_color);
        setXY(line_thickness, center_point.x_coordinate + y, center_point.y_coordinate - x, circle_color);
        setXY(line_thickness, center_point.x_coordinate - y, center_point.y_coordinate - x, circle_color);
        
        if (decision < 0) {
            decision += 2 * x + 3;
        } else {
            decision += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}
