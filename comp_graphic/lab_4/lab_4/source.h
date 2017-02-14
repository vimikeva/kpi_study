#pragma once
#include <glut.h> 
#include <gl/Gl.h>
#include <gl/Glu.h>
#include <iostream>
#include <windows.h>
#include <vector>
#include <cmath>
#include <ctime>

#define HEIGHT 700
#define WIDTH 1350

using namespace std;

int algorizm = 1;
int size1 = pow(3, 5) / 1.3, step1 = 1; //koch
int step2 = 1; // sierpinski


class Point {
public:
	int x;
	int y;

	Point() : x(0), y(0) {};
	Point(int x, int y) : x(x), y(y) {};
	Point& operator=(Point& p) {
		if (this == &p)
			return *this;
		x = p.x;
		y = p.y;
		return *this;
	}

	friend const bool operator==(Point& p, const Point& P) {
		if (&p == &P)
			return true;
		if (p.x == P.x && p.y == P.y)
			return true;
		return false;
	}

};


void draw_line(Point, Point);
void draw_triangle(Point, Point, Point);
void draw();
void fractal_koch(float, int, int);
void fractal_sierpinski(Point *, int);
void mandelbrot_set();
void mandelbrot_could();
