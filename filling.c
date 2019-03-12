#include "filling.h"

// global variable
char available[1000][700];

void init_avail() {
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 700; ++j) {
            available[i][j] = 0;
            // printf("%d\n", available[i][j]);
        }
    }
}

/*
Procedure floodfill

fp_x    : fire point x
fp_y    : fire point y
C   : warna flood
fc  : warna dasar yang akan ditimpa oleh warna flood
*/

void floodFill(int fp_x, int fp_y, Color C, Color fc) {

    if (isColorSame(C, fc)) {
        return;
    }

    queue pq_d;
    queue* pq = &pq_d;

    initQueue(pq);

    insertPoint(pq, makePoint(fp_x, fp_y));

    setXY(1, fp_x, fp_y, C);

    while(!queueEmpty(pq)) {
        Point p = nextPoint(pq);
        if ((p.x>=0) && (p.x<(vinfo.xres-1)) && (p.y>=0) && (p.y<(vinfo.yres-7))) {
            if (isColorSame(fc, getXY(p.x, p.y-1)) == 1) {
                setXY(1, p.x, p.y-1, C);
                insertPoint(pq, makePoint(p.x, p.y-1));
            }

            if (isColorSame(fc, getXY(p.x+1, p.y)) == 1) {
                setXY(1, p.x+1, p.y, C);
                insertPoint(pq, makePoint(p.x+1, p.y));
            }

            if (isColorSame(fc, getXY(p.x, p.y+1)) == 1) {
                setXY(1, p.x, p.y+1, C);
                insertPoint(pq, makePoint(p.x, p.y+1));
            }

            if (isColorSame(fc, getXY(p.x-1, p.y)) == 1) {
                setXY(1, p.x-1, p.y, C);
                insertPoint(pq, makePoint(p.x-1, p.y));
            }
        }
    }
}

void raster_fill(int y_min, int y_max, int x_min, int x_max) {

    for (int i = y_min; i < y_max; ++i) {
        for (int j = x_min; j < x_max; ++j) {

        if (available[j][i] == 0)
            if (isColorSame(getXY(j, i), setColor(255,255,255))) { // iteration one line in minimapand view

                // Start here
                if (isColorSame(getXY(j+1,i), setColor(255,255,255))) { // building
                    while (isColorSame(getXY(j+1,i), setColor(255,255,255))) {
                        j++;
                    }
                    j++;
                } else if (isColorSame(getXY(j+1,i), setColor(0,0,0))) { // building not filled
                    while (isColorSame(getXY(j+1,i), setColor(0,0,0))) {
                        setXY(1, j, i, setColor(255,255,255));
                        if (i >= 0 && j >= 0)
                            available[i][j] = 1;
                        j++;
                    }
                    j++;
                }
            }
            //  else if (isColorSame(getXY(j, i), setColor(255,0,0))) { // iteration one line in clipping window
            //     while (!isColorSame(getXY(j+1, i), setColor(255,0,0)) && !isColorSame(getXY(j-1, i), setColor(255,0,0))) {
            //         setXY(1, j+1, i, setColor(255,0,0));
            //         available[i][j] = 1;
            //     }
                
            // }
            // else if (isColorSame(getXY(j, i), setColor(0,))) { // iteration one line in clipping window
        }
    }
}