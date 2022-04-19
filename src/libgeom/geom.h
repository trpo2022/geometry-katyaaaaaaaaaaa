#ifndef GEOM_H
#define GEOM_H
#include <inttypes.h>
typedef struct Circle {
    double x;
    double y;
    double radius;
    double perimeter;
    double area;
} Circle;

typedef struct Point {
    double x;
    double y;
} Point;

typedef struct Triangle {
    Point t1;
    Point t2;
    Point t3;
    double perimeter;
    double area;
} Triangle;

typedef struct CircleArray {
    Circle* elems;
    int num;
} CircleArray;

typedef struct TriangleArray {
    Triangle* elems;
    int num;
} TriangleArray;

void toLowerCase(char* str, int size);
void underline_error_position(const uint8_t space_count);
void printErrorMessage(
        int errorPosition, const char* expected, const char* after);

int findWord(char** pointer, char* figureName, const char* word);
int findSymbol(char** pointer, char symbol);

int setDouble(char** pointer, double* doublePtr);
int setXY(char** pointer, double* x, double* y);
int setRadius(char** pointer, double* r);

int checkLineEnd(char** pointer);
int notOnlySpaces(char* const start, char* const end);

double circlePerimeter(double r);
double trianglePerimeter(double a, double b, double c);
double circleArea(double r);
double triangleArea(double a, double b, double c);
double segmentLength(double x1, double x2, double y1, double y2);

int setCircle(char** pointer, Circle* circle, int errorPosition);
int setTriangle(char** pointer, Triangle* triangle, int errorPosition);
void calculateCircle(Circle* circle);
void calculateTriangle(Triangle* triangle);

void printCircle(Circle* circle, char* figureName);
void printTriangle(Triangle* triangle, char* figureName);

CircleArray* createCircleArray(void);
void deleteCircleArray(CircleArray*);
void addCircle(CircleArray*);
void removeCircle(CircleArray*);
void printCircleArray(CircleArray*);

TriangleArray* createTriangleArray(void);
void deleteTriangleArray(TriangleArray*);
void addTriangle(TriangleArray*);
void removeTriangle(TriangleArray*);
void printTriangleArray(TriangleArray*);

int isInto(double, double, double);
int isIntersectTT(Triangle*, Triangle*);
int isIntersectTC(Triangle*, Circle*);
int isIntersectCC(Circle*, Circle*);
int isIntersectSegments(Point*, Point*, Point*, Point*);
int isIntersectTSegment(Point*, Point*, Triangle*);
int isIntersectCSegment(Point*, Point*, Circle*);

void printAll(CircleArray*, TriangleArray*);
#endif
