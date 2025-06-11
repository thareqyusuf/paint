#include "framebuffer.h"
#include <stdlib.h>

struct color_rgba create_rgba_color(int red, int green, int blue) {
    struct color_rgba color;
    color.red_component = red;
    color.green_component = green;
    color.blue_component = blue;
    color.alpha_component = 255; // Full opacity by default
    return color;
}

// Legacy function name compatibility
struct color_rgba setColor(int r, int g, int b) {
    return create_rgba_color(r, g, b);
}

int are_colors_similar(struct color_rgba first_color, struct color_rgba second_color) {
    int offset = 1;
    if ((abs(first_color.red_component - second_color.red_component) <= offset) && 
        (abs(first_color.green_component - second_color.green_component) <= offset) && 
        (abs(first_color.blue_component - second_color.blue_component) <= offset)) {
        return 1;
    }
    return 0;
}

// Legacy function name compatibility
int isColorSame(struct color_rgba C1, struct color_rgba C2) {
    return are_colors_similar(C1, C2);
}
