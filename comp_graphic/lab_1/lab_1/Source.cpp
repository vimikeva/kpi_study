#include <iostream>
#include <windows.h>
#include <string.h>
#include <glut.h>
#include <ctime>

#define MINX 0
#define MAXX 1300
#define MINY 0
#define MAXY 700
#define SIZE 4

using namespace std;

int menu();
void draw_vect_dda();
void draw_vect_Bre();
void draw_vect_Vu();
void draw_circle_Bre();
void draw_ellipse_Bre();
void draw_surname();
void time();

void initialize(){
	glClearColor(0.8, 1.0, 0.6, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(MINX, MAXX, MAXY, MINY);

}

int main(int argc, char **argv) {

	int ch = menu();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(MAXX, MAXY);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Lab 1");
	
	switch (ch) {
	case 1: {
		glutDisplayFunc(draw_vect_dda);
		break; }
	case 2: {
		glutDisplayFunc(draw_vect_Bre);
		break; }
	case 3: {
		glutDisplayFunc(draw_vect_Vu);
		break; }
	case 4: {
		glutDisplayFunc(draw_circle_Bre);
		break; }
	case 5: {
		glutDisplayFunc(draw_ellipse_Bre);
		break; }
	case 6: {
		glutDisplayFunc(draw_surname);
		break; }
	case 7: {
		time();
		break; }
	}
	initialize();
	glutMainLoop();

	return 0;
}

void vect_dda(int x1, int y1, int x2, int y2) {
	float Px = x2 - x1;
	float Py = y2 - y1;

	glVertex2i(x1*SIZE, y1*SIZE);
	while (x1  <  x2) {
		x1 = x1 + 1;
		y1 = y1 + Py / Px;
		glVertex2i(x1*SIZE, y1*SIZE);
	}
}

void draw_vect_dda() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(SIZE);
	glBegin(GL_POINTS);
	glColor3ub(0, 0, 0);
	int coordx[] = { MAXX - 1, MAXX - 1, MAXX - 1, MAXX / 2 - 1, 1 },
		coordy[] = { 1, MAXY / 2 - 1, MAXY - 1, MAXY - 1, MAXY - 1 };
	for (int i = 0; i < 5; i++)
		vect_dda(1, 1, coordx[i], coordy[i]);
	glEnd();
	glFlush();
}

void vect_Bre(int x1, int y1, int x2, int y2) {
	int deltaX = abs(x2 - x1);
	int deltaY = abs(y2 - y1);
	int signX = x1 < x2 ? 1 : -1;
	int signY = y1 < y2 ? 1 : -1;
	
	int error = deltaX - deltaY;
	
	glVertex2i(x2*SIZE, y2*SIZE);
	while (x1 != x2 || y1 != y2)
	 {
		glVertex2i(x1*SIZE, y1*SIZE);
		int error2 = error * 2;
		
		if (error2 > -deltaY){
			error -= deltaY;
			x1 += signX;
		}
		if (error2 < deltaX){
			error += deltaX;
			y1 += signY;
		}
	}
}

void draw_vect_Bre() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(SIZE);
	glBegin(GL_POINTS);
	glColor3ub(0, 0, 0);
	int coordx[] = { MAXX - 1, MAXX - 1, MAXX - 1, MAXX / 2 - 1, 1 },
		coordy[] = { 1, MAXY / 2 - 1, MAXY - 1, MAXY - 1, MAXY - 1 };
	for (int i = 0; i < 5; i++)
		vect_Bre(1, 1, coordx[i], coordy[i]);
	glEnd();
	glFlush();
}

void vect_Vu(int x1, int y1, int x2, int y2) {
	int  dx, dy, sx, sy, kl, swap;
	long incr1, incr2;
	long s;
	long s_max;
	long color_tek, Pix_V = 255, Pix_C = 0;
	int  xt;

	sx = 0;
	if ((dx = x1 - x2) < 0) { dx = -dx; --sx; }
	else if (dx>0) ++sx;
	sy = 0;
	if ((dy = y1 - y2) < 0) { dy = -dy; --sy; }
	else if (dy>0) ++sy;

	swap = 0;
	if ((kl = dx) < (s = dy)) { dx = s;  dy = kl;  kl = s; ++swap; }
	s = (long)dx*(long)Pix_V;
	incr1 = 2l * (long)dy       
		*(long)Pix_V;   
	incr2 = 2l * s;             
								
	s_max = incr2 - incr1;    
	color_tek = Pix_V;        
	if (dx)color_tek = (int)((((long)Pix_V*(long)dy) / (long)dx) / 2l);
	glVertex2d(x2, y2);      
	while (--kl >= 0) {
		if (s >= s_max) {
			if (swap) x2 += sx; else y2 += sy;
			s -= incr2;
		}
		if (swap) y2 += sy; else x2 += sx;
		s += incr1;
		color_tek = Pix_V;
		if (dx) color_tek = s / dx / 2;
		glVertex2d(x2*SIZE, y2*SIZE);     
												
		xt = x2;
		while (++xt <= x1) 
			glVertex2d(xt*SIZE, y2*SIZE);
	}
}

void draw_vect_Vu() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(SIZE);
	glBegin(GL_POINTS);
	glColor3ub(0, 0, 0);
	int coordx[] = { MAXX - 1, MAXX - 1, MAXX - 1, MAXX / 2 - 1, 1 },
		coordy[] = { 1, MAXY / 2 - 1, MAXY - 1, MAXY - 1, MAXY - 1 };
	for (int i = 0; i < 5; i++)
		vect_Vu(1, 1, coordx[i], coordy[i]);
	glEnd();
	glFlush();
}

void circle_Bre(int _x, int _y, int radius){
	int x = 0, y = radius, gap = 0, delta = (2 - 2 * radius);
	while (y >= 0)
	{
		glVertex2i((_x + x)*SIZE, (_y + y)*SIZE);
		glVertex2i((_x + x)*SIZE, (_y - y)*SIZE);
		glVertex2i((_x - x)*SIZE, (_y - y)*SIZE);
		glVertex2i((_x - x)*SIZE, (_y + y)*SIZE);
		gap = 2 * (delta + y) - 1;
		if (delta < 0 && gap <= 0)
		{
			x++;
			delta += 2 * x + 1;
			continue;
		}
		if (delta > 0 && gap > 0)
		{
			y--;
			delta -= 2 * y + 1;
			continue;
		}
		x++;
		delta += 2 * (x - y);
		y--;
	}
}

void draw_circle_Bre() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(SIZE);
	glBegin(GL_POINTS);
	glColor3ub(0, 0, 0);
	for (int i = 1; i < 6; i++)
		circle_Bre(MAXX/2/SIZE, MAXY/2/SIZE, i*MAXY/12/SIZE);
	glEnd();
	glFlush();
}

void ellipse_Bre(int x, int y, int a, int b)
{
	int col, row;
	long a_square, b_square, two_a_square, two_b_square, four_a_square, four_b_square, d;

	b_square = b*b;
	a_square = a*a;
	row = b;
	col = 0;
	two_a_square = a_square << 1;
	four_a_square = a_square << 2;
	four_b_square = b_square << 2;
	two_b_square = b_square << 1;
	d = two_a_square*((row - 1)*(row)) + a_square + two_b_square*(1 - a_square);
	while (a_square*(row)>b_square*(col))
	{
		glVertex2i((col + x)*SIZE, (row + y)*SIZE);
		glVertex2i((col + x)*SIZE, (y - row)*SIZE);
		glVertex2i((x - col)*SIZE, (row + y)*SIZE);
		glVertex2i((x - col)*SIZE, (y - row)*SIZE);
		if (d >= 0)
		{
			row--;
			d -= four_a_square*(row);
		}
		d += two_b_square*(3 + (col << 1));
		col++;
	}
	d = two_b_square*(col + 1)*col + two_a_square*(row*(row - 2) + 1) + (1 - two_a_square)*b_square;
	while ((row)+1)
	{
		glVertex2i((col + x)*SIZE, (row + y)*SIZE);
		glVertex2i((col + x)*SIZE, (y - row)*SIZE);
		glVertex2i((x - col)*SIZE, (row + y)*SIZE);
		glVertex2i((x - col)*SIZE, (y - row)*SIZE);
		if (d <= 0)
		{
			col++;
			d += four_b_square*col;
		}
		row--;
		d += two_a_square*(3 - (row << 1));
	}
}

void draw_ellipse_Bre() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(SIZE);
	glBegin(GL_POINTS);
	glColor3ub(0, 0, 0);
	for (int i = 1; i < 6; i++)
		ellipse_Bre(MAXX / 2 / SIZE, MAXY / 2 / SIZE,i*MAXX / 12 / SIZE, i*MAXY / 12 / SIZE);
	glEnd();
	glFlush();
}

void draw_surname() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(SIZE);
	glBegin(GL_POINTS);
	glColor3ub(0, 0, 0);
	int x = 14, y = MAXY / 2 / SIZE;

	char *text = "Vitovshchyk Mykhailo";
	for (int i = 0; text[i] != '\0'; i++) {
		switch (text[i]) {
		case ' ': {
			x += 8;
			break;
		}
		case 'V': {
			vect_Bre(x + 8, y, x, y - 30);
			vect_Bre(x + 8, y, x + 16, y - 30);
			x += 16;
			break; }
		case 'i': {
			vect_Bre(x + 5, y, x + 5, y - 15);
			vect_Bre(x + 5, y - 20, x + 5, y - 22);
			x += 9;
			break; }
		case 't': {
			vect_Bre(x + 5, y - 2, x + 5, y - 28);
			vect_Bre(x + 5, y - 2, x + 7, y);
			vect_Bre(x + 7, y, x + 10, y);
			vect_Bre(x + 1, y - 20, x + 9, y - 18);
			x += 14;
			break; }
		case 'o': {
			ellipse_Bre(x + 5, y - 6, 5, 6);
			x += 14;
			break; }
		case 'v': {
			vect_Bre(x + 5, y, x, y - 12);
			vect_Bre(x + 5, y, x + 10, y - 12);
			x += 14;
			break; }
		case 's': {
			vect_Bre(x, y - 2, x + 2, y);
			vect_Bre(x + 2, y, x + 4, y);
			vect_Bre(x + 4, y, x + 6, y - 2);
			vect_Bre(x + 6, y - 2, x + 6, y - 4);
			vect_Bre(x + 6, y - 4, x + 4, y - 6);
			vect_Bre(x + 4, y - 6, x + 2, y - 6);
			vect_Bre(x + 2, y - 6, x, y - 8);
			vect_Bre(x, y - 8, x, y - 10);
			vect_Bre(x, y - 10, x + 2, y - 12);
			vect_Bre(x + 2, y - 12, x + 4, y - 12);
			vect_Bre(x + 4, y - 12, x + 6, y - 10);
			x += 10;
			break; }
		case 'h': {
			vect_Bre(x, y, x, y - 28);
			vect_Bre(x, y - 12, x + 4, y - 12);
			vect_Bre(x + 4, y - 12, x + 7, y - 10);
			vect_Bre(x + 7, y - 10, x + 7, y);
			x += 10;
			break; }
		case 'c': {
			vect_Bre(x + 6, y, x + 2, y);
			vect_Bre(x + 2, y, x, y - 2);
			vect_Bre(x, y - 2, x, y - 10);
			vect_Bre(x, y - 10, x + 2, y - 12);
			vect_Bre(x + 2, y - 12, x + 6, y - 12);
			x += 10;
			break; }
		case 'y': {
			vect_Bre(x + 4, y, x, y - 12);
			vect_Bre(x + 6, y - 12, x + 2, y + 12);
			x += 10;
			break; }
		case 'k': {
			vect_Bre(x, y, x, y - 28);
			vect_Bre(x, y - 6, x + 6, y);
			vect_Bre(x, y - 6, x + 6, y - 12);
			x += 10;
			break; }
		case 'l': {
			vect_Bre(x + 2, y, x + 2, y - 28);
			x += 5;
			break; }
		case 'M': {
			vect_Bre(x, y, x, y - 30);
			vect_Bre(x, y - 30, x + 6, y - 18);
			vect_Bre(x + 6, y - 18, x + 12, y - 30);
			vect_Bre(x + 12, y - 30, x + 12, y);
			x += 16;
			break; }
		case 'a': {
			vect_Bre(x, y - 12, x + 4, y - 12);
			vect_Bre(x + 4, y - 12, x + 6, y - 10);
			vect_Bre(x + 6, y - 10, x + 6, y);
			vect_Bre(x + 6, y, x + 2, y);
			vect_Bre(x + 2, y, x, y - 2);
			vect_Bre(x, y - 2, x, y - 4);
			vect_Bre(x, y - 4, x + 2, y - 6);
			vect_Bre(x + 2, y - 6, x + 6, y - 6);
			x += 8;
			break; }
		}
	}
	glEnd();
	glFlush();
}

void time() {
	int n = 10000;
	unsigned int time = clock();
	for (int i = 0; i < n; i++)
		vect_dda(MINX, MINY, MAXX, MAXY);
	time = clock() - time;
	cout << "Vector DDA time - " << time << endl;
	
	time = clock();
	for (int i = 0; i < n; i++)
		vect_Bre(MINX, MINY, MAXX, MAXY);
	time = clock() - time;
	cout << "Vector Bre time - " << time << endl;

	time = clock();
	for (int i = 0; i < n; i++)
		vect_Vu(MINX, MINY, MAXX, MAXY);
	time = clock() - time;
	cout << "Vector Vu time - " << time << endl;

	time = clock();
	for (int i = 0; i < n; i++)
		circle_Bre(MINX/2, MINY/2, MAXX/2);
	time = clock() - time;
	cout << "Circle Bre time - " << time << endl;

	time = clock();
	for (int i = 0; i < n; i++)
		ellipse_Bre(MINX/2, MINY/2, MAXX/2, MAXY/2);
	time = clock() - time;
	cout << "Ellipse Bre time - " << time << endl;

	system("pause");
}

int menu() {
	int ch;
	do {
		cout << "Menu:\n\n";
		cout << "1. Vector DDA\n";
		cout << "2. Vector Brethenkhema\n";
		cout << "3. Vector Vu\n";
		cout << "4. Circle Brethenkhema\n";
		cout << "5. Ellipse Brethenkhema\n";
		cout << "6. Surname\n";
		cout << "7. Times\n";
		cout << "0. Exit\n\n";

		cout << "Make a choice: ";
		cin >> ch;

		if (ch < 0 || ch > 7) {
			cout << "not correct!\n";
			system("pause");
			system("clr");
		}
	} while (ch < 0 || ch > 7);
	return ch;
}

