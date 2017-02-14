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
	int x = WIDTH - 100, y = HEIGHT - 20;

	glColor3i(0, 0, 0);
	glBegin(GL_LINES);
	for (int i = y-20; i < HEIGHT-3; i++) {
		glVertex2i(x, i);
		glVertex2d(WIDTH-3, i);
	}
	glEnd();
	glColor3f(0, 0, 1);
	
	glRasterPos2i(x, y);
	if (mode == 0) str = "MODE: 0";
	else str = "MODE: 1";
	for (int i = 0; i < (int)str.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
	
	glRasterPos2i(x, y - 20);
	if (algorizm == 1) str = "ALG: 1";
	if (algorizm == 2) str = "ALG: 2";
	if (algorizm == 3) str = "ALG: 3";
	for (int i = 0; i < (int)str.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	glColor3f(0, 1, 0);
	glFlush();
}

void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
	  if (mode == 1) {
		  glColor3f(1.0, 0.0, 0.0);
		  if (algorizm == 1)
			  alg_one(&Point(x, HEIGHT - y));
		  if (algorizm == 2)
			  alg_two(Point(x, HEIGHT - y));
		  if (algorizm == 3)
			  alg_three(Point(x, HEIGHT - y));
	  }
	  else {
		  if (algorizm == 1) {
			  display();
			  draw();

			  Point *points = new Point[NUMBER_POINTS];
			  for (int i = 0; i < NUMBER_POINTS; i++)
				  points[i] = Point(rand() % WIDTH, rand() % HEIGHT);

			  for (int i = 0; i < NUMBER_POINTS; i++)
				  for (int j = i + 1; j < NUMBER_POINTS; j++) {
					  glBegin(GL_LINES);
					  glVertex2i(points[i].x, points[i].y);
					  glVertex2i(points[j].x, points[j].y);
					  glEnd();
					  glFlush();
				  }
		  }
		  if (algorizm == 2 || algorizm == 3) {
			  int zona[] = {300, 100,
							300, 600,
							1000, 600,
							1000, 100};
			  display();
			  draw();
			  glBegin(GL_LINES);
			  for (int i = 0; i < 6; i+=2) 
				  draw_line(Point(zona[i], zona[i + 1]), Point(zona[i + 2], zona[i + 3]));
				draw_line(Point(zona[0], zona[1]), Point(zona[6], zona[7]));
			  glEnd();
			  glFlush();

			  int n = 5;
			  Point *points = new Point[4];
			  for (int i = 0, x, y; i < n; i++) {
				  while ((x = rand() % WIDTH) < 300 || x > 1000);
				  while ((y = rand() % HEIGHT) < 100 || y > 600);
				  points[i] = Point(x, y);
			  }
			  glBegin(GL_LINES);
			  for (int i = 0; i < n - 1; i++)
				  draw_line(Point(points[i].x, points[i].y), Point(points[i + 1].x, points[i + 1].y));
			  draw_line(Point(points[0].x, points[0].y), Point(points[n-1].x, points[n-1].y));
			  glEnd();
			  glFlush();
		  }
	  }

  }
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
	  if (mode)
		  mode = 0;
	  else 
		  mode = 1;
	  glPrintStatus();
  }
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		algorizm = 1;
		break;
	case '2':
		algorizm = 2;
		break;
	case '3':
		algorizm = 3;
		break;
	}
	glPrintStatus();
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Bezier Curve");

    glutMouseFunc(mouse);
    glutDisplayFunc(draw);
	glutKeyboardFunc(keyboard);

    initialization();
    glutMainLoop();

    return 0;
}

void draw_dot (Point p) {
	glBegin(GL_POINTS);
	 glVertex2i(p.x, p.y);
	glEnd();
	glFlush();
}

void draw_line(Point p1, Point p2) {
	glBegin(GL_LINES);
	 glVertex2i(p1.x, p1.y);
	 glVertex2i(p2.x, p2.y);
	glEnd();
	glFlush();
}

void draw() {
	glColor3f(0, 1, 0);

	glBegin(GL_LINES);
	 glVertex2i(1, 1);
	 glVertex2i(1, HEIGHT-1);
	 glVertex2i(1, HEIGHT-1);
	 glVertex2i(WIDTH, HEIGHT-1);
	 glVertex2i(WIDTH, HEIGHT-1);
	 glVertex2i(WIDTH, 1);
	 glVertex2i(WIDTH, 1);
	 glVertex2i(1, 1);
	glEnd();
	glPrintStatus();
	glFlush();
}

void alg_one(Point *p) {
	GLubyte color[3];
	glReadPixels(p->x, p->y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);

	if (color[0] == color[1] == color[2] == 0) {
		draw_dot(*p);

		alg_one(&Point(p->x + 1, p->y));
		alg_one(&Point(p->x, p->y + 1));
		alg_one(&Point(p->x - 1, p->y));
		alg_one(&Point(p->x, p->y - 1));
	}
}

void alg_two(Point p) {
	glColor3f(1, 0, 0);
	GLubyte color[3];
	for (int y = 601; y > 100; y--) {
		bool f = false;
		draw_dot(Point(1003, y));
		for (int x = 300; x < 999; x++) {
			glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);

			if ((int)color[0] ==0 && (int)color[2] == 0 && (int)color[1] == 255) {
				if (!f) {
					while ((int)color[0] ==0 && (int)color[2] == 0 && (int)color[1] == 255)
						glReadPixels(++x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);
					p = Point(x, y);
				}
				else {
					draw_line(p, Point(x, y));
					while ((int)color[0] == 0 && (int)color[2] == 0 && (int)color[1] == 255)
						glReadPixels(++x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);
				}
				f = !f;
			}
		}
	}
}

void alg_three(Point p) {
	stack<Point> stack_points;
	stack_points.push(p);
	GLubyte color[3];
	cout << "start " << (int)color[0] << "-" << (int)color[1] << "-" << (int)color[2] << endl;
	while (!stack_points.empty()) {
		p = stack_points.top();
		stack_points.pop();
		glReadPixels(p.x, p.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);
		if ((int)color[0] == 0 && (int)color[1] == 0 && (int)color[2] == 0) {
			draw_dot(p);
			stack_points.push(Point(p.x + 1, p.y));
			stack_points.push(Point(p.x - 1, p.y));
			stack_points.push(Point(p.x, p.y + 1));
			stack_points.push(Point(p.x, p.y - 1));
		}
	}
}