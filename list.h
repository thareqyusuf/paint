#include <stdio.h>
#include "point.h"

typedef struct node {
    struct node * first;
    Point val;
    struct node * next;
} list_of_point;

typedef struct node_point
{
  struct node_point * first;
  list_of_point lo_point;
	struct node_point * next;
} list_of_polygon;

typedef struct node_poly
{
  struct node_poly * first;
  list_of_polygon lo_poly;
	struct node_poly * next;
} list_of_layer;

#define First(L) L.first
#define Next(L) L.next
#define Current(L) L.val

void create(); // allocate from point to canvas
void destroy(); // deallocate from canvas to point
void add(Point p); // add the most basic element, Point.
void traverse(); // go to next node on list of point
int is_empty(); // checks the emptyness of a list