////=====lab_2

#include "source.h"

using namespace std;

void initialization() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(4.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
	display();
	points.push_back(Point(x, HEIGHT - y));
	draw_curve();
  }
}

void keyboard(int key, int x, int y) {
	vector<Point> vect;
	double temp_x, temp_y;
	switch (key) {
	case GLUT_KEY_LEFT:
		for (int i = 0; i < points.size(); i++)
			vect.push_back(Point(points[i].X() - WIDTH / 2, points[i].Y() - HEIGHT / 2));
		for (int i = 0; i < vect.size(); i++) {
			temp_x = -vect[i].Y() + WIDTH / 2;
			temp_y = vect[i].X() + HEIGHT /2;
			points[i] = Point(temp_x, temp_y);
		}
		display();
		draw_curve();
		break;
	case GLUT_KEY_RIGHT:
		for (int i = 0; i < points.size(); i++)
			vect.push_back(Point(points[i].X() - WIDTH / 2, points[i].Y() - HEIGHT / 2));
		for (int i = 0; i < vect.size(); i++) {
			temp_x = vect[i].Y() + WIDTH / 2;
			temp_y = -vect[i].X() + HEIGHT / 2;
			points[i] = Point(temp_x, temp_y);
		}
		display();
		draw_curve();
		break;
	case GLUT_KEY_UP:
		for (size_t i = 0; i < points.size(); ++i)
			points[i] = Point(
				 1.2 *(points[i].X() - x) + WIDTH/2,
				 1.2 *(points[i].Y() - (HEIGHT - y)) + HEIGHT/2);
		display();
		draw_curve();
		break;
	case GLUT_KEY_DOWN:
		for (size_t i = 0; i < points.size(); ++i)
			points[i] = Point(
				0.8 * (points[i].X() - x) + WIDTH/2,
				0.8 * (points[i].Y() - (HEIGHT - y)) + HEIGHT/2);
		display();
		draw_curve();
		break;
	}
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Bezier Curve");

    glutMouseFunc(mouse);
    glutDisplayFunc(display);
	glutSpecialFunc(keyboard);

    initialization();
    glutMainLoop();

    return 0;
}

void draw_dot(Point p) {
	glBegin(GL_POINTS);
	 glVertex2i(p.X(), p.Y());
	glEnd();
	glFlush();
}

void draw_line(Point p1, Point p2) {
	glBegin(GL_LINES);
	 glVertex2f(p1.X(), p1.Y());
	 glVertex2f(p2.X(), p2.Y());
	glEnd();
	glFlush();
}

Point point_bezier(vector<Point>& pts, double t) {
	Point p;
	for (size_t i = 0; i < pts.size(); i++)
		p += pts[i] * choose(pts.size() - 1, i) * pow(1 - t, pts.size() - 1 - i) * pow(t, i);
	return p;
}

long long factorial (long long num) {
	if (num <= 1)
		return 1;
	else
		return num * factorial(num - 1);
}

double choose (long long a, long long b) {
	return factorial(a) / (double)(factorial(b) * factorial(a - b));
}

void draw_curve() {
	glColor3f(1, 0, 0);
	for (size_t i = 0; i < points.size(); i++)
		draw_dot(points[i]);

	double delta_t = 0.001;

	if (points.size() >= 2) {
		glColor3f(1.0, 1.0, 1.0);

		for (int i = 1; i < points.size(); ++i)
			//draw_line(points[i - 1], points[i]);

		glColor3f(0, 1.0, 0);
		Point POld = points[0];
		for (double t = 0.0; t <= 1.0; t += delta_t) {
			Point P = point_bezier(points, t);
			
			draw_line(POld, P);
			POld = P;
		}
	}
}