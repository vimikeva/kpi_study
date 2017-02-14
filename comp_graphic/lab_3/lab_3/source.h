#pragma once
#include <glut.h> 
#include <gl/Gl.h>
#include <gl/Glu.h>
#include <iostream>
#include <windows.h>
#include <stack>
#include <algorithm>
#include <ctime>

using namespace std;

#define HEIGHT 700
#define WIDTH 1350
#define NUMBER_POINTS 15

int algorizm = 1;
int mode = 0;

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

void draw_dot(Point);
void draw_line(Point, Point);
void draw();
void alg_one(Point *);
void alg_two(Point);
void alg_three(Point);
