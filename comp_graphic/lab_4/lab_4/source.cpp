#include "source.h"

void initialization() {
	glClearColor(0, 0, 0, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void glPrintStatus(){
	string str;
	int x = WIDTH -200 , y = HEIGHT - 30;
	if (algorizm == 1)
		glColor3f(0, 0, 1);
	else if (algorizm == 2)
		glColor3f(0, 1, 0);
	else if (algorizm == 3)
		glColor3f(1, 0, 0);
	else
		glColor3f(0, 1, 1);

	glBegin(GL_LINES);
	for (int i = y-15; i < HEIGHT-3; i++) {
		glVertex2i(x-10, i);
		glVertex2d(WIDTH-3, i);
	}
	glEnd();

	glColor3f(1, 1, 1);
	glRasterPos2i(x, y);
	if (algorizm == 1) str = "Fractal Koch's";
	if (algorizm == 2) str = "Fractal Sierpinski's";
	if (algorizm == 3) str = "Set Mandelbrot's";
	if (algorizm == 4) str = "Cold Mandelbrot's";
	for (int i = 0; i < (int)str.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	glFlush();
}

void mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		Point p[3] = { Point(200, 50), Point(900, 50), Point(550, 650) };
		display();
		if (button == GLUT_LEFT_BUTTON) {
			if (algorizm == 1) {
				step1++;
				size1 /= 3;
				if (step1 == 6) {
					step1 = 1;
					size1 = pow(3, 5) / 1.3;
				}
			}
			else if (algorizm == 2) {
				step2++;
				if (step2 == 9)
					step2 = 1;
			}
		}
		else if (button == GLUT_RIGHT_BUTTON) {
			algorizm++;
			if (algorizm == 5)
				algorizm = 1;
		}

		if (algorizm == 1) {
			//blue
			glPrintStatus();
			glColor3f(0, 0, 1);

			fractal_koch(0.0, size1, step1);
			fractal_koch(-120.0, size1, step1);
			fractal_koch(120.0, size1, step1);
		}
		else if (algorizm == 2) {
			//green
			glPrintStatus();
			glColor3f(0, 1, 0);
			draw_triangle(p[0], p[1], p[2]);
			glColor3f(0, 0, 0);
			fractal_sierpinski(p, step2);
		}
		else if (algorizm == 3) {
			//red
			glPrintStatus();
			glColor3f(1, 0, 0);
			mandelbrot_set();
			glPrintStatus();
		}
		else if (algorizm == 4) {
			glPrintStatus();
			glColor3f(0, 1, 1);
			mandelbrot_could();
			glPrintStatus();
		}
	}
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Lab 4");

    glutMouseFunc(mouse);
    glutDisplayFunc(draw);

    initialization();
    glutMainLoop();

    return 0;
}

void draw_dot(Point p, GLubyte *color) {
	glColor3d(color[0], color[1], color[2]);
	glBegin(GL_POINTS);
	glVertex2i(p.x, p.y);
	glEnd();
}

void draw_dotb(Point p, GLubyte *color) {
	glColor3b(color[0], color[1], color[2]);
	glBegin(GL_POINTS);
	glVertex2i(p.x, p.y);
	glEnd();
}

void draw_line(Point p1, Point p2) {
	glBegin(GL_LINES);
	 glVertex2i(p1.x, p1.y);
	 glVertex2i(p2.x, p2.y);
	glEnd();
	glFlush();
}

void draw_triangle(Point p1, Point p2, Point p3) {
	glBegin(GL_TRIANGLES);
	 glVertex2d(p1.x, p1.y);
	 glVertex2d(p2.x, p2.y);
	 glVertex2d(p3.x, p3.y);
	glEnd();
	glFlush();
}

void draw() {
	mouse(-1, 0, 0, 0);
	glFlush();
}

GLfloat oldx = 300, oldy = 520;
void fractal_koch(float dir, int len, int iter) {
	GLdouble dirRad = 0.0174533 * dir;
	GLfloat newX = oldx + len * cos(dirRad);
	GLfloat newY = oldy + len * sin(dirRad);
	if (iter == 0) {
		draw_line(Point(oldx, oldy), Point(newX, newY));
		oldx = newX;
		oldy = newY;
	}
	else {
		iter--;
		fractal_koch(dir, len, iter);
		fractal_koch(dir + 60, len, iter);
		fractal_koch(dir - 60, len, iter);
		fractal_koch(dir, len, iter);
	}
}

void fractal_sierpinski(Point *p, int step) {
	if (step > 0) {
		Point p1[3], p2[3], p3[3];
		p1[0] = p[0];
		p1[1] = p2[0] = Point((p[0].x + p[1].x) / 2, (p[0].y + p[1].y) / 2);
		p1[2] = p3[0] = Point((p[2].x + p[0].x) / 2, (p[2].y + p[0].y) / 2);
		p2[1] = p[1];
		p2[2] = p3[1] = Point((p[1].x + p[2].x) / 2, (p[1].y + p[2].y) / 2);
		p3[2] = p[2];

		draw_triangle(p1[1], p2[2], p3[0]);

		fractal_sierpinski(p1, --step);
		fractal_sierpinski(p2, step);
		fractal_sierpinski(p3, step);
	}
}

GLubyte* hsv_to_rgb(GLubyte h, float s, float v) {
	GLubyte *rgb = new GLubyte[3];
	int c = v * s,
		x = c * (1 - abs((h / 60) % 2 - 1)),
		m = v - c;
	if (h < 60) {
		rgb[0] = c;
		rgb[1] = x;
		rgb[2] = 0;
	}
	else if (h < 120) {
		rgb[0] = x;
		rgb[1] = c;
		rgb[2] = 0;
	}
	else if (h < 180) {
		rgb[0] = 0;
		rgb[1] = c;
		rgb[2] = x;
	}
	else if (h < 240) {
		rgb[0] = 0;
		rgb[1] = x;
		rgb[2] = c;
	}
	else if (h < 300) {
		rgb[0] = x;
		rgb[1] = 0;
		rgb[2] = c;
	}
	else {
		rgb[0] = c;
		rgb[1] = 0;
		rgb[2] = x;
	}
	rgb[0] = (rgb[0] + m) * 255;
	rgb[1] = (rgb[1] + m) * 255;
	rgb[2] = (rgb[2] + m) * 255;
	return rgb;
}

void mandelbrot_set() {
	int h = HEIGHT, w = WIDTH;
	double pr, pi;
	double newRe, newIm, oldRe, oldIm;
	double zoom = 1, moveX = -0.5, moveY = 0;
	GLubyte *color;
	int maxIterations = 300;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			pr = 1.5 * (x - w / 2) / (0.5 * zoom * w) + moveX;
			pi = (y - h / 2) / (0.5 * zoom * h) + moveY;
			newRe = newIm = oldRe = oldIm = 0;
			int i;
			for (i = 0; i < maxIterations; i++) {
				oldRe = newRe;
				oldIm = newIm;
				newRe = oldRe * oldRe - oldIm * oldIm + pr;
				newIm = 2 * oldRe * oldIm + pi;
				if ((newRe * newRe + newIm * newIm) > 4) break;
			}
			color = hsv_to_rgb(i % 256, 1, (i < maxIterations));

			draw_dot(Point(x, y), color);
			delete[] color;
		}
		glFlush();
	}
}

void mandelbrot_could() {
	double s = 0.0023;
	int iter = 500, BAIL_OUT = 16, step = 10, dx = -720, dy = -350;
	Point *vertex = new Point[iter];

	for (int i = 0; i < WIDTH; i += step) {
		for (int j = 0; j < HEIGHT; j += step) {
			double c = s * (i + dx), d = s * (j + dy);
			double x = c, y = d, t;
			int k = 0;

			while (x * x + y * y < BAIL_OUT && k < iter) {
				t = x * x - y * y + c;
				y = 2 * x * y + d;
				x = t;
				vertex[k] = Point(x / s - dx, y / s - dy);
				++k;
			}

			if (k == iter) {
				GLubyte *color = new GLubyte[3];
				color[0] = rand() % 256;
				color[1] = rand() % 256;
				color[2] = rand() % 256;

				for (int i = 1; i < k; i++)
					draw_dotb(vertex[i], color);
				delete[] color;
			}
		}
		Sleep(100);
		glFlush();
	}

}