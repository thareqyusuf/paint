#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <sys/time.h>

// Include our unified headers
#include "framebuffer.h"


int fbfd = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize = 0;
char *fbp = 0;
int displayWidth, displayHeight;


// global variable

char available[3840][2160];

void init_avail() {
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 700; ++j) {
            available[i][j] = 0;
            // printf("%d\n", available[i][j]);
        }
    }
}

/*
Initiate connection to framebuffer
*/
void initScreen() {
    init_avail();
    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    if (vinfo.yres < 700) {
        screensize = vinfo.xres * finfo.line_length * vinfo.bits_per_pixel / 8;
    } else {
        screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    }

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
                fbfd, 0);
    if ((long)fbp == -1) {
    perror("Error: failed to map framebuffer device to memory");
    exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");
    displayWidth = vinfo.xres;
    displayHeight = vinfo.yres;
}

/*Color struct consists of Red, Green, and Blue */
/*
Procedure untuk menggambar ke framebuffer

squareSize    : square size
x   : x coordinate
y   : y coordinate
C       : Color struct (Red, Green, Blue)
*/
void setXY (int squareSize, int x, int y, struct color_rgba C) {
    long int location;
    int i,j;
    if (((x)>=0) && ((x + squareSize)<vinfo.xres) && ((y)>=0) && ((y + squareSize)<vinfo.yres)) {
        for (i = x; i < (x+squareSize); i++) {
            for (j = y; j < (y+squareSize); j++) {
                location = (i+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (j+vinfo.yoffset) * finfo.line_length;

                if (fbp + location) { //check for segmentation fault
                    if (vinfo.bits_per_pixel == 32) {
                        *(fbp + location) = C.B;            //Blue
                        *(fbp + location + 1) = C.G;        //Green
                        *(fbp + location + 2) = C.R;        //Red
                        *(fbp + location + 3) = 0;          //Transparancy
                    } else  { //assume 16bpp
                        int r = C.R;     //Red
                        int g = C.G;     //Green
                        int b = C.B;     //Blue

                        unsigned short int t = r<<11 | g << 5 | b;
                        *((unsigned short int*)(fbp + location)) = t;
                    }
                    available[x][y] = 1;
                } else {
                    return;
                }
            }
        }
    }
}



/*
Set screen background with C color
*/
void printBackground(struct color_rgba C) {
    long int location;
    int width = displayWidth - 6;
    int height = displayHeight - 6;
    int i,j;

    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            location = (i+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (j+vinfo.yoffset) * finfo.line_length;
            if (vinfo.bits_per_pixel == 32) {
                *(fbp + location) = C.B;         //Blue
                *(fbp + location + 1) = C.G;     //Green
                *(fbp + location + 2) = C.R;     //Red
                *(fbp + location + 3) = 0;       //No transparency
            } else  { //assume 16bpp
                int r = C.R;     //Red
                int g = C.G;     //Green
                int b = C.B;     //Blue

                unsigned short int t = r<<11 | g << 5 | b;
                *((unsigned short int*)(fbp + location)) = t;
            }
        }
    }
}



/*
Function untuk mendapatkan warna dari suatu pixel pada posisi x dan y
*/
struct color_rgba getXY(int x, int y) {
    long int location;
    Color out;
    out.R = -999; out.G = -999; out.B = -999;
    if (((x)>=0) && (x<vinfo.xres) && ((y)>=0) && (y<vinfo.yres)) {

        location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;

        if (fbp + location) { //check for segmentation fault
            if (vinfo.bits_per_pixel == 32) {

                out.R = (int) *(fbp + location + 2);        //Red
                out.G = (int) *(fbp + location + 1);        //Green
                out.B = (int) *(fbp + location);            //Blue
                if (out.R < 0) out.R += 256;
                if (out.G < 0) out.G += 256;
                if (out.B < 0) out.B += 256;

                *(fbp + location + 3) = 0;          //Transparancy
            } else  { //assume 16bpp
                int t = *((unsigned short int*)(fbp + location));
                out.R = (t & 63488) >> 11;     //Red
                out.G = (t & 2016) >> 5;     //Green
                out.B = t & 31;     //Blue

            }
        }
    } else {
    }
    return out;
}


/*
Closing the framebuffer connection
*/
void terminate(){
    munmap(fbp, screensize);
    close(fbfd);
}
// Bridge functions for new interface compatibility
int initialize_framebuffer_system(void) {
    initScreen();
    return 0;
}

void fill_background_color(struct color_rgba background_color) {
    printBackground(background_color);
}

void set_pixel_with_thickness(int thickness, int x_coordinate, int y_coordinate, 
                             struct color_rgba pixel_color) {
    setXY(thickness, x_coordinate, y_coordinate, pixel_color);
}

struct color_rgba get_pixel_color_at_position(int x_coordinate, int y_coordinate) {
    return getXY(x_coordinate, y_coordinate);
}

void terminate_framebuffer_system(void) {
    terminate();
}

// Additional function implementations
int initialize_framebuffer_system(void) {
    return initScreen();
}

void terminate_framebuffer_system(void) {
    terminate();
}

struct color_rgba create_rgba_color(int red, int green, int blue) {
    struct color_rgba color;
    color.R = (uint8_t)red;
    color.G = (uint8_t)green;
    color.B = (uint8_t)blue;
    color.A = 255;
    return color;
}

int are_colors_similar(struct color_rgba first_color, struct color_rgba second_color) {
    return (first_color.R == second_color.R && 
            first_color.G == second_color.G && 
            first_color.B == second_color.B);
}

// Additional missing function implementations
struct coordinate_point create_coordinate_point(int x, int y) {
    struct coordinate_point point;
    point.x_coordinate = x;
    point.y_coordinate = y;
    return point;
}

void draw_filled_polygon(int vertex_count, struct coordinate_point *vertex_array, 
                        struct color_rgba fill_color, int outline_thickness) {
    // Simple polygon fill - just draw lines between vertices
    for (int i = 0; i < vertex_count; i++) {
        int next = (i + 1) % vertex_count;
        draw_line_between_points(vertex_array[i], vertex_array[next], fill_color, outline_thickness);
    }
}

void draw_connected_polyline(int point_count, struct coordinate_point* coordinate_array, 
                           struct color_rgba line_color, int line_thickness) {
    // Draw connected lines
    for (int i = 0; i < point_count - 1; i++) {
        draw_line_between_points(coordinate_array[i], coordinate_array[i+1], line_color, line_thickness);
    }
}
