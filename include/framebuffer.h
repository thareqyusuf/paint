#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "color.h"
#include "point.h"
#include "grafika.h"


extern int displayHeight;
extern int displayWidth;
extern char *fbp;
extern long int screensize;
extern struct fb_fix_screeninfo finfo;
extern struct fb_var_screeninfo vinfo;
extern int fbfd;
extern Point explosionPoint[11];

extern char available[3840][2160];

//basic framebuffer primitive
void init_avail();
void initScreen();
void printBackground(Color C);
void setXY (int squareSize, int x, int y, Color C);
Color getXY(int x, int y);
void terminate();


#endif