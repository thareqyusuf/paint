#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

// Unified color structure with both field name conventions
struct color_rgba {
    union {
        struct {
            uint8_t red_component;
            uint8_t green_component; 
            uint8_t blue_component;
            uint8_t alpha_component;
        };
        struct {
            uint8_t R;
            uint8_t G;
            uint8_t B;
            uint8_t A;
        };
    };
};

// Provide compatibility typedefs
typedef struct color_rgba Color;
typedef struct color_rgba color_rgba;

// Helper function to create colors
static inline struct color_rgba make_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    struct color_rgba c;
    c.R = r; c.G = g; c.B = b; c.A = a;
    return c;
}

#endif // COLOR_H
