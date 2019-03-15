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

unsigned char vKey = 0;
pthread_t keypressListener;

int polyCount = 0;
int* polyType;
Point* polyPoints1;
Point* polyPoints2;
Point* points;
Point* polygons[10];
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

		/*for (j = 0 ; j < (isPoly ? nPoints : nPoints - 1) ;j++){
			int lawan = (j+1);
			if (j == (nPoints-1)) lawan = 0;

			LineAnalysisResult lar1;
			//kasus khusus kalau menaik vertikal
			if ((points[j].x == points[lawan].x) && (points[lawan].y > points[j].y)) {
					lar1 = analyzeLine(points[lawan] , points[j], cw1);
			}
			else {
					lar1 = analyzeLine(points[j] , points[lawan], cw1);
			}
			Point clippedLine[2];
			clipLine(lar1,cw1,clippedLine);
			if (isLineSane(2, clippedLine)) {
				drawPolyline(2,clippedLine,setColor(255,0,0),1);
				ScaleLine(clippedLine,xScalingFactor,yScalingFactor);
				TranslationLine(clippedLine,xTranslation,yTranslation);

				drawPolyline(2,clippedLine,color,1);

			}
		}*/
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
	/*if (rotationDegree > 0) {
		tes = rotateMany(center, clippingWindow, rotationDegree, 4);
		int i;
		for(i = 0; i < 4; i++) {
			clippingWindow[i] = tes[i];
		}
	}*/
	drawPolygon(4,clippingWindow,colors[currentColor],4);
	
	Point viewingWindow[4];
	
	viewingWindow[0] = makePoint(50,50);
	viewingWindow[1] = makePoint(50,550);
	viewingWindow[2] = makePoint(1000,550);
	viewingWindow[3] = makePoint(1000,50);
	
	// drawPolygon(4,viewingWindow,setColor(0,255,180),1);

	//FITUR 1
	Point featureWindow1[4];
	
	featureWindow1[0] = makePoint(60,60);
	featureWindow1[1] = makePoint(60,110);
	featureWindow1[2] = makePoint(110,110);
	featureWindow1[3] = makePoint(110,60);
	if (fill) {
		drawPolygon(4,featureWindow1,setColor(0,100,180),1);
	} else {
		drawPolygon(4,featureWindow1,setColor(0,255,180),1);
	}
	

	//Fitur 2
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
	
	

	//Fitur 3
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

	//Fitur 4
	Point featureWindow4[4];

	featureWindow3[0] = makePoint(240,60);
	featureWindow3[1] = makePoint(240,110);
	featureWindow3[2] = makePoint(290,110);
	featureWindow3[3] = makePoint(290,60);
	if (vKey) {
		drawPolygon(4,featureWindow3,setColor(0,100,180),1);
	} else {
		drawPolygon(4,featureWindow3,setColor(0,255,180),1);
	}



	//DRAW ALL POLY
	// for (int i = 0; i< polyCount; i++) {
	// 	if (polyType[i] == 1) {
	// 		drawCircle(100, makePoint(polyPoints1[i].x, polyPoints1[i].y), 1, setColor(100, 100, 100));
	// 	}
	// }

	for (int i = 0; i < 10; i++) {
		drawPolygon(polygonsP[i].neff, polygonsP[i].p, polygonsP[i].c, 1);
		// floodFill(pointsColor[i].x, pointsColor[i].y, pointsColorChoice[i], setColor(0, 0, 0));
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
						// polygons[polyCount][i] = points[i];
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
					// polygons[polyCount][i] = points[i];
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



	

	//CEK INPUT PRESSED

	//if (drawBuildings) refreshFromFile("building.txt", 1, setColor(255,255,255));
	//if (drawRoads) refreshFromFile("jalan.txt", 0,setColor(255,255,0));
	//if (drawTrees) refreshFromFile("pohon.txt", 1,setColor(0,255,0));

	//raster_fill(50, 550, 500, 1000);
	//raster_fill(scaleFactor*(100+up), scaleFactor*(200+up), scaleFactor*(100+left), scaleFactor*(200+left));

	
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
				polygonsP[i].p[j].x = polygonsP[i].p[j].x * 2 / 3;
				polygonsP[i].p[j].y = polygonsP[i].p[j].y * 2 / 3;
			}
		}
	}
}

void scaleUp() {
	for (int i = 0; i < polyCount; i++) {
		if (isInside(left, up, polygonsP[i])) {
			for (int j = 0; j < polygonsP[i].neff; j++) {
				polygonsP[i].p[j].x = polygonsP[i].p[j].x * 3 / 2;
				polygonsP[i].p[j].y = polygonsP[i].p[j].y * 3 / 2;
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
		else if (cmd == V_KEYPRESS) {vKey = !vKey; refreshScreen();}
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
	// polyType = malloc(10 * sizeof(int));
	// polyPoints1 = malloc(10 * sizeof(Point));
	// polyPoints2 = malloc(10 * sizeof(Point));
	points = malloc(10 * sizeof(Point));
	// for (int i = 0; i < 10; i++) {
	// 	polygons[i] = (Point *)malloc(10 * sizeof(Point));
	// }

	initScreen();
	initColors();
	printBackground(setColor(0,0,0));

	printBackground(setColor(0,0,0));
	refreshScreen();
	pthread_create(&keypressListener, NULL, keypressListen, NULL);
	programBarrier();
	terminate();
	return 0;
}

/*
void showSplashScreen() {
	//Initiate Splash Screen Component
	Point * hurufI = (Point*) malloc(5 * sizeof(Point));
	Point * hurufT = (Point*) malloc(9 * sizeof(Point));
	Point * hurufB = (Point*) malloc(7 * sizeof(Point));
	Point * detailHurufB1 = (Point*) malloc(5 * sizeof(Point));
	Point * detailHurufB2 = (Point*) malloc(5 * sizeof(Point));
	Point * bintang = (Point*) malloc (5 * sizeof(Point));
	Point * kotak = (Point *) malloc(sizeof(Point) * 4);
	Point * persegi = (Point *) malloc(sizeof(Point) * 4);
	Polygon * polygons = (Polygon *) malloc(sizeof(Polygon) * 5);


	persegi[0] = makePoint(275, 525);
	persegi[1] = makePoint(275, 575);
	persegi[2] = makePoint(325, 575);
	persegi[3] = makePoint(325, 525);
	polygons[0].p = persegi;
	polygons[0].neff = 4;
	polygons[0].c = setColor(255,0,0);
	polygons[0].center = makePoint(300,550);
	kotak[0] = makePoint(250, 500);
	kotak[1] = makePoint(250, 590);
	kotak[2] = makePoint(400, 590);
	kotak[3] = makePoint(400, 500);
	polygons[1].p = kotak;
	polygons[1].neff = 4;
	polygons[1].c = setColor(255,255,0);
	polygons[1].center = makePoint(325,545);

	int baseX = 400;
	int baseY = 300;
	//center = base+min+((max-min)/2)
	Point centerI;
	Point centerT;
	Point centerB;
	Point centerB1;
	Point centerB2;
	int isBounce = 0;
	int iteration = 0;
	while (1) {
		hurufI[0].x = baseX + 25; hurufI[0].y = baseY;
		hurufI[1].x = baseX + 50; hurufI[1].y = baseY;
		hurufI[2].x = baseX + 50; hurufI[2].y = baseY + 100;
		hurufI[3].x = baseX + 25; hurufI[3].y = baseY + 100;
		hurufI[4].x = baseX + 25; hurufI[4].y = baseY;
		rotationDegreeText = (rotationDegreeText+10)%360;
		centerI.x = baseX+37; centerI.y = baseY+50;
		hurufI = rotateMany(centerI,hurufI,rotationDegreeText,5);

		hurufT[0].x = baseX + 70; hurufT[0].y = baseY;
		hurufT[1].x = baseX + 145; hurufT[1].y = baseY;
		hurufT[2].x = baseX + 145; hurufT[2].y = baseY + 20;
		hurufT[3].x = baseX + 115; hurufT[3].y = baseY + 20;
		hurufT[4].x = baseX + 115; hurufT[4].y = baseY + 100;
		hurufT[5].x = baseX + 100; hurufT[5].y = baseY + 100;
		hurufT[6].x = baseX + 100; hurufT[6].y = baseY + 20;
		hurufT[7].x = baseX + 70; hurufT[7].y = baseY + 20;
		hurufT[8].x = baseX + 70; hurufT[8].y = baseY;
		centerT.x = baseX+107; centerT.y = baseY+60;
		hurufT = rotateMany(centerT,hurufT,rotationDegreeText,9);

		hurufB[0].x = baseX + 165; hurufB[0].y = baseY;
		hurufB[1].x = baseX + 215; hurufB[1].y = baseY;
		hurufB[2].x = baseX + 215; hurufB[2].y = baseY + 50;
		hurufB[3].x = baseX + 265; hurufB[3].y = baseY + 50;
		hurufB[4].x = baseX + 265; hurufB[4].y = baseY + 100;
		hurufB[5].x = baseX + 165; hurufB[5].y = baseY + 100;
		hurufB[6].x = baseX + 165; hurufB[6].y = baseY;
		centerB.x = baseX+215; centerB.y=baseY+75;
		hurufB = rotateMany(centerB,hurufB,rotationDegreeText,7);

		detailHurufB1[0].x = baseX + 180; detailHurufB1[0].y = baseY + 20;
		detailHurufB1[1].x = baseX + 180; detailHurufB1[1].y = baseY + 40;
		detailHurufB1[2].x = baseX + 200; detailHurufB1[2].y = baseY + 40;
		detailHurufB1[3].x = baseX + 200; detailHurufB1[3].y = baseY + 20;
		detailHurufB1[4].x = baseX + 180; detailHurufB1[4].y = baseY + 20;
		// centerB1.x = baseX+190; centerB1.y = baseY+30;
		detailHurufB1 = rotateMany(centerB,detailHurufB1,rotationDegreeText,5);

		detailHurufB2[0].x = baseX + 180; detailHurufB2[0].y = baseY + 65;
		detailHurufB2[1].x = baseX + 180; detailHurufB2[1].y = baseY + 85;
		detailHurufB2[2].x = baseX + 250; detailHurufB2[2].y = baseY + 85;
		detailHurufB2[3].x = baseX + 250; detailHurufB2[3].y = baseY + 65;
		detailHurufB2[4].x = baseX + 180; detailHurufB2[4].y = baseY + 65;
		// centerB2.x = baseX+215; centerB2.y = baseY+75;
		detailHurufB2 = rotateMany(centerB,detailHurufB2,rotationDegreeText,5);

		if (hurufB[3].x >= displayWidth) {
			isBounce = 1;
			iteration++;
		}
		if (hurufI[0].x <= 0) {
			isBounce = 0;
			iteration++;
		}
		if (isBounce == 0) {
			baseX += 100;
		} else {
			baseX -= 100;
		}
		drawPolyline(5, hurufI, setColor(255,255,255), 1);
		floodFill(centerI.x, centerI.y, setColor(255,0,0), setColor(0,0,0));
		drawPolyline(9, hurufT, setColor(255,255,255), 1);
		floodFill(centerT.x, centerT.y, setColor(0,255,0), setColor(0,0,0));
		drawPolyline(7, hurufB, setColor(255,255,255), 1);
		floodFill(centerB.x, centerB.y, setColor(0,0,255), setColor(0,0,0));
		drawPolyline(5, detailHurufB1, setColor(255,255,255), 1);
		drawPolyline(5, detailHurufB2, setColor(255,255,255), 1);
		int polygonIdx = 0;
		drawPolygon(polygons[polygonIdx].neff, polygons[polygonIdx].p, polygons[polygonIdx].c, 1);
		floodFill(polygons[polygonIdx].center.x, polygons[polygonIdx].center.y, polygons[polygonIdx].c, setColor(0,0,0));
		for (polygonIdx = 1; polygonIdx < 2; ++polygonIdx) {
			drawPolygon(polygons[polygonIdx].neff, polygons[polygonIdx].p, polygons[polygonIdx].c, 1);
			floodFill(polygons[polygonIdx].center.x, polygons[polygonIdx].center.y, polygons[polygonIdx].c, setColor(0,0,0));
		}
		printBackground(setColor(0,0,0));
		if (iteration == 3) {
			break;
		}
	}
}*/