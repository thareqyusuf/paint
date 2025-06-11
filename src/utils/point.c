
struct coordinate_point create_coordinate_point(int x_coordinate, int y_coordinate) {
    struct coordinate_point point;
    point.x_position = x_coordinate;
    point.y_position = y_coordinate;
    return point;
}

// Legacy function name compatibility
struct coordinate_point makePoint(int x, int y) {
    return create_coordinate_point(x, y);
}
