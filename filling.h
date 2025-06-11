#ifndef FILLING_H
#define FILLING_H

#include "point.h"
#include "color.h"
#include "framebuffer.h"
#include "pointqueue.h"

void floodFill(int fp_x, int fp_y, Color C, Color fc);
void flood(int x, int y, Color newColor, Color oldColor);
void raster_fill(int y_min, int y_max, int x_min, int x_max);

#endif
