#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include "color.h"
#include "point.h"

// Screen and framebuffer variables
extern char *fbp;
extern int fbfd;
extern int screensize;
extern int displayWidth;
extern int displayHeight;
extern int displayDepth;
extern int finfo_line_length;
extern int vinfo_xres;
extern int vinfo_yres;
extern int vinfo_bits_per_pixel;

// Core framebuffer functions
int initScreen(void);
void terminate(void);
void setXY(int squareSize, int x, int y, struct color_rgba C);
struct color_rgba getXY(int x, int y);
void printBackground(struct color_rgba C);

// Modern API functions
void fill_background_color(struct color_rgba background_color);
void set_pixel_with_thickness(int thickness, int x_coordinate, int y_coordinate, struct color_rgba pixel_color);
struct color_rgba get_pixel_color_at_position(int x_coordinate, int y_coordinate);

#endif // FRAMEBUFFER_H
