#include "grafika.h"
#include "color.h"
#include "point.h"
#include "framebuffer.h"
#include "geometry.h"
#include "transform.h"
#include "filling.h"
#include "clipping.h"
#include "keypress.h"

typedef struct Polygons {
	Color c;
	Point p[10];
	int neff;
} Polygon;

double left = 500;
double up = 250;
double scaleFactor = 1;
int rotationDegree = 0;
int rotationDegreeText = 0;
int pointCount = 0;
int pointColorCount = 0;
double scaling = 1;
unsigned char inputPress = 0;
unsigned char fill = 0;
unsigned char drawS = 0;
unsigned char drawR = 0;
unsigned char drawT = 0;

pthread_t keypressListener;

int polyCount = 0;
Point* points;
Polygon polygonsP[10];
Color colors[4];
int currentColor;
Point pointsColor[10];
Color pointsColorChoice[10];



unsigned char between(int a, int b, int x) {
	return x > a && x < b;
}

unsigned char isLineSane(int n, Point* line) {
	for (size_t i = 0; i < n; i++) {
		// Should be fine for a few years
		if (!between(-50000, 100000, line[i].x) || !between(-50000, 100000, line[i].y)) return 0;
	}
	return 1;
}

void refreshFromFile (const char* filename, unsigned char isPoly, Color color) {
	
	FILE* fp = fopen(filename, "r");

	ClippingWindow cw1 = setClippingWindow(scaleFactor*(100+left),scaleFactor*(200+left),scaleFactor*(200+up),scaleFactor*(100+up));
	double xScalingFactor = (1000 - 500) / ( (scaleFactor*(200+left)) - (scaleFactor*(100+left)) );
	double yScalingFactor = (550 - 50) / ( scaleFactor*((200+left)) - scaleFactor*((100+left)));
	double xTranslation = (( (scaleFactor*(200+left)) * 500) - ( (scaleFactor*(100+left)) * 1000)) / ( (scaleFactor*(200+left)) - (scaleFactor*(100+left)));
	double yTranslation = (( (scaleFactor*(200+up)) * 50) - ( (scaleFactor*(100+up)) * 550)) / ( (scaleFactor*(200+up)) - (scaleFactor*(100+up)) );	

	while(!feof(fp)){
		
		int nPoints;
		fscanf(fp, "%d", &nPoints);
		Point * points = malloc(nPoints * sizeof(Point));

		
		int j;
		for(j = 0 ; j < nPoints ; j++) fscanf(fp, "%d %d", &points[j].x, &points[j].y); 

		if (isPoly) drawPolygon(j,points,color,1);
		else drawPolyline(j,points,color,1);

		free(points);
	}
	close(fileno(fp));
}

int isInside(int x, int y, Polygon checkedPoly) {
	int maxY = 0;
	int maxX = 0;
	int minY = 99999;
	int minX = 99999;
	for (int i = 0; i < checkedPoly.neff; i++) {
		if (maxX < checkedPoly.p[i].x) {
			maxX = checkedPoly.p[i].x;
		}
		if (minX > checkedPoly.p[i].x) {
			minX = checkedPoly.p[i].x;
		}
		if (maxY < checkedPoly.p[i].y) {
			maxY = checkedPoly.p[i].y;
		}
		if (minY > checkedPoly.p[i].y) {
			minY = checkedPoly.p[i].y;
		}

	}

	if (maxX >= x && minX <= x && minY <= y && maxY >= y) {
		return 1;
	} else {
		return 0;
	}

}

//redraw framebuffer
void refreshScreen() 
{

	printBackground(setColor(0,0,0));
	// Point clippingWindow[4];
	Point *clippingWindow;
	Point *tes;
	Point center;
	clippingWindow = (Point *) malloc(4 * sizeof(Point));
	clippingWindow[0] = makePoint(scaleFactor*(0+left),scaleFactor*(0+up));
	clippingWindow[1] = makePoint(scaleFactor*(0+left),scaleFactor*(1+up));
	clippingWindow[2] = makePoint(scaleFactor*(1+left),scaleFactor*(1+up));
	clippingWindow[3] = makePoint(scaleFactor*(1+left),scaleFactor*(0+up));
	center = makePoint(scaleFactor*(0+left),scaleFactor*(150+up));
	
	drawPolygon(4,clippingWindow,colors[currentColor],4);
	
	Point viewingWindow[4];
	
	viewingWindow[0] = makePoint(50,50);
	viewingWindow[1] = makePoint(50,550);
	viewingWindow[2] = makePoint(1000,550);
	viewingWindow[3] = makePoint(1000,50);

	//Fitur DrawTriangle
	Point featureWindow2[4];
	
	featureWindow2[0] = makePoint(120,60);
	featureWindow2[1] = makePoint(120,110);
	featureWindow2[2] = makePoint(170,110);
	featureWindow2[3] = makePoint(170,60);
	if (drawT) {
		drawPolygon(4,featureWindow2,setColor(0,100,180),1);
	} else {
		drawPolygon(4,featureWindow2,setColor(0,255,180),1);
	}
	
	

	//Fitur DrawRectangle
	Point featureWindow3[4];
	
	featureWindow3[0] = makePoint(180,60);
	featureWindow3[1] = makePoint(180,110);
	featureWindow3[2] = makePoint(230,110);
	featureWindow3[3] = makePoint(230,60);
	if (drawR) {
		drawPolygon(4,featureWindow3,setColor(0,100,180),1);
	} else {
		drawPolygon(4,featureWindow3,setColor(0,255,180),1);
	}




	for (int i = 0; i < 10; i++) {
		drawPolygon(polygonsP[i].neff, polygonsP[i].p, polygonsP[i].c, 1);
		flood(pointsColor[i].x, pointsColor[i].y, pointsColorChoice[i], setColor(0, 0, 0));
	}

	//CEK PENGGUNAAN FITUR FILL
	if (fill) {
		pointsColor[pointColorCount].x = left;
		pointsColor[pointColorCount].y = up;
		pointsColorChoice[pointColorCount] = colors[currentColor];
		pointColorCount++;
		fill = !fill;
		refreshScreen();
	}

	if (drawR) {
		int cmd = getch();

		if (cmd == SPACE_KEYPRESS) {
			if (pointCount == 0) {
				points[pointCount] = makePoint(left, up);
				pointCount++;
			} else {
				points[pointCount] = makePoint(points[0].x, up); pointCount++;
				points[pointCount] = makePoint(left, up); pointCount++;
				points[pointCount] = makePoint(left, points[0].y); pointCount++;

				if (pointCount == 4) {
					for (int i = 0; i < pointCount; i++) {
						
						polygonsP[polyCount].p[i] = points[i];
						polygonsP[polyCount].neff = pointCount;
						polygonsP[polyCount].c = colors[currentColor];
					}
					for (int i = 0; i < pointCount; i++) {
						points[i].x = NULL;
						points[i].y = NULL;
					}
					pointCount = 0;
					polyCount++;
					drawR = !drawR;
					refreshScreen();
				}
			}
		}
	}

	if (drawT) {
		int cmd = getch();

	 	if (cmd == SPACE_KEYPRESS) {
			points[pointCount] = makePoint(left, up);
			pointCount++;
			if (pointCount == 3) {
				for (int i = 0; i < pointCount; i++) {
					polygonsP[polyCount].p[i] = points[i];
					polygonsP[polyCount].neff = pointCount;
					polygonsP[polyCount].c = colors[currentColor];
				}
				for (int i = 0; i < pointCount; i++) {
						points[i].x = NULL;
						points[i].y = NULL;
					}
				pointCount = 0;
				polyCount++;
				drawT = !drawT;
				refreshScreen();
			}
		}
	}

}

void zoomOut() {
	for (int i = 0; i < polyCount; i++) {
		for (int j = 0; j < polygonsP[i].neff; j++) {
			polygonsP[i].p[j].x = polygonsP[i].p[j].x * 9 / 10;
			polygonsP[i].p[j].y = polygonsP[i].p[j].y * 9 / 10;
		}
	}
}

void zoomIn() {
	for (int i = 0; i < polyCount; i++) {
		for (int j = 0; j < polygonsP[i].neff; j++) {
			polygonsP[i].p[j].x = polygonsP[i].p[j].x * 10 / 9;
			polygonsP[i].p[j].y = polygonsP[i].p[j].y * 10 / 9;
		}
	}
}

void rotatePolyLeft() {
	for (int i = 0; i < polyCount; i++) {
		if (isInside(left, up, polygonsP[i])) {
			for (int j = 0; j < polygonsP[i].neff; j++) {
				float s = sin(-10 * 3.14 / 180.0);
				float c = cos(-10 * 3.14 / 180.0);
				polygonsP[i].p[j].x -= left;
				polygonsP[i].p[j].y -= up;

				int xnew = (int) (polygonsP[i].p[j].x * c - polygonsP[i].p[j].y * s);
				int ynew = (int) (polygonsP[i].p[j].x * s + polygonsP[i].p[j].y * c);

				polygonsP[i].p[j].x = xnew + left;
				polygonsP[i].p[j].y = ynew + up;
			}
		}
	}
}

void rotatePolyRight() {
	for (int i = 0; i < polyCount; i++) {
		if (isInside(left, up, polygonsP[i])) {
			for (int j = 0; j < polygonsP[i].neff; j++) {
				float s = sin(-10 * 3.14 / 180.0);
				float c = cos(-10 * 3.14 / 180.0);
				polygonsP[i].p[j].x -= left;
				polygonsP[i].p[j].y -= up;

				int xnew = (int) (polygonsP[i].p[j].x * c - polygonsP[i].p[j].y * s);
				int ynew = (int) (polygonsP[i].p[j].x * s + polygonsP[i].p[j].y * c);

				polygonsP[i].p[j].x = xnew + left;
				polygonsP[i].p[j].y = ynew + up;
			}
		}
	}
}

void scaleDown() {
	for (int i = 0; i < polyCount; i++) {
		if (isInside(left, up, polygonsP[i])) {
			for (int j = 0; j < polygonsP[i].neff; j++) {
				polygonsP[i].p[j].x = polygonsP[i].p[j].x * 9 / 10;
				polygonsP[i].p[j].y = polygonsP[i].p[j].y * 9 / 10;
			}
		}
	}
}

void scaleUp() {
	for (int i = 0; i < polyCount; i++) {
		if (isInside(left, up, polygonsP[i])) {
			for (int j = 0; j < polygonsP[i].neff; j++) {
				polygonsP[i].p[j].x = polygonsP[i].p[j].x * 10 / 9;
				polygonsP[i].p[j].y = polygonsP[i].p[j].y * 10 / 9;
			}
		}
	}
}


//keypress listener in separate thread
void *keypressListen(void *x_void_ptr) {
	 int cmd = ' ';
	 while (1) 
	 {
	 	cmd = getch();
	 	if (cmd == LEFT_KEYPRESS) { left -= 20; refreshScreen();}
	    else if (cmd == RIGHT_KEYPRESS) {	left+= 20;refreshScreen();	}
	    else if (cmd == UP_KEYPRESS ) {up += 20; refreshScreen();}
	    else if (cmd == DOWN_KEYPRESS ) {up -= 20; refreshScreen();}
	    else if (cmd == ZOOMIN_KEYPRESS) {scaleFactor -= 0.1; refreshScreen();}
	    else if (cmd == ZOOMOUT_KEYPRESS) {scaleFactor += 0.1; refreshScreen();}
	    else if (cmd == Z_KEYPRESS) {fill = !fill; refreshScreen();}
	    else if (cmd == X_KEYPRESS) {drawT = !drawT; refreshScreen();}
	    else if (cmd == C_KEYPRESS) {drawR = !drawR; refreshScreen();}
	    else if (cmd == R_KEYPRESS) {
	    	rotationDegree = (rotationDegree + 10) % 360;
	    	refreshScreen();
		}
		else if (cmd == LESSTHAN_KEYPRESS) {
			if (currentColor == 0) {
				currentColor = 3;
			} else {
				currentColor--;
			}
			refreshScreen();
		}
		else if (cmd == MORETHAN_KEYPRESS) {
			if (currentColor == 3) {
				currentColor = 0;
			} else {
				currentColor++;
			}
			refreshScreen();
		} else if (cmd == O_KEYPRESS) {zoomOut(); refreshScreen();}
		else if (cmd == P_KEYPRESS) {zoomIn(); refreshScreen();}
		else if (cmd == E_KEYPRESS) {rotatePolyLeft(); refreshScreen();}
		else if (cmd == R_KEYPRESS) {rotatePolyRight(); refreshScrenn();}
		else if (cmd == K_KEYPRESS) {scaleDown(); refreshScreen();}
		else if (cmd == L_KEYPRESS) {scaleUp(); refreshScreen();}
	}
	return NULL;
}

//prevents exiting program
void programBarrier(){
	while(1) {}
}

void initColors() {
	colors[0] = setColor(255, 255, 255);
	colors[1] = setColor(255, 0, 0);
	colors[2] = setColor(0, 255, 0);
	colors[3] = setColor(0, 0, 255);
}

int main() {
	points = malloc(10 * sizeof(Point));
	initScreen();
	initColors();
	printBackground(setColor(0,0,0));
	refreshScreen();
	pthread_create(&keypressListener, NULL, keypressListen, NULL);
	programBarrier();
	terminate();
	return 0;
}
