#pragma once

#include "point.h"

using std::cerr;
using std::endl;
#include <stdlib.h>
// using std::exit;
#include <windows.h>
#include <glut.h> 
#include <gl/Gl.h>
#include <gl/Glu.h>
#include <cmath>
#include <vector>

#define HEIGHT 700
#define WIDTH 1350

std::vector<Point> points;

Point point_bezier(std::vector<Point>& pts, double t);
long long factorial(long long m);
double choose(long long a, long long b);
void draw_dot(Point p);
void draw_line(Point p1, Point p2);
void draw_curve();