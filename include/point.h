#ifndef POINT_H
#define POINT_H

// Unified point structure with both naming conventions
struct point_2d {
    int x_coordinate;
    int y_coordinate;
};

struct coordinate_point {
    int x_coordinate;
    int y_coordinate;
};

// Provide compatibility typedefs
typedef struct point_2d Point;
typedef struct coordinate_point point_2d;

// Helper function to create points
static inline struct point_2d make_point(int x, int y) {
    struct point_2d p;
    p.x_coordinate = x;
    p.y_coordinate = y;
    return p;
}

#endif // POINT_H
