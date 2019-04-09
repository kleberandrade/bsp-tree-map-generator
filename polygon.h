#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <stdio.h>

typedef struct _Point2 {
    int x;
    int y;
} Point2;

Point2 BuildPoint2(int x, int y){
    Point2 point;
    point.x = x;
    point.y = y;
    return point;
}

void PrintPoint(const Point2 point){
    printf("Point [%d, %d]\n", point.x, point.y);
}

typedef struct _Rectangle {
    int x;
    int y;
    int w;
    int h;
} Rectangle;

Rectangle BuildRectangle(int x, int y, int w, int h){
    Rectangle rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    return rect;
}

void PrintRectangle(const Rectangle rect){
    printf("Rectangle [%d, %d, %d, %d]\n", rect.x, rect.y, rect.w, rect.h);
}

#endif // __POLYGON_H__
