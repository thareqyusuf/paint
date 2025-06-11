#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "graphics.h"

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

#endif // FRAMEBUFFER_H
