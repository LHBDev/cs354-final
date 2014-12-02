#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <sys/time.h>
#include <iostream>
#include <stdio.h>

typedef class vertex 	*Vertex;
typedef class line 	 	*Line;

/***************************************************/
/************** Vertex Class ***********************/
/***************************************************/

class vertex {
		float x;
		float y;
		float z;
	public:
		vertex (float, float, float);
		void set_coordinates(float, float, float);
		void draw();
};

vertex::vertex(float x1, float y1, float z1){
	x = x1;
	y = y1;
	z = z1;
}

void vertex::set_coordinates(float x1, float y1, float z1){
	x = x1;
	y = y1;
	z = z1;
}

void vertex::draw(){
	glPointSize(10.0f);
	glBegin(GL_POINTS);
		glVertex3f(x, y, z);
	glEnd();
}

/***************************************************/
/********** End Vertex Class ***********************/
/***************************************************/

std::vector <vertex> vertices;

static void Init(){
	glClearColor(0.3, 0.3, 0.3, 0.0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	vertex a(1.0f, 1.0f, 0.0f);
	vertices.push_back(a);
}

static void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -4.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glutSwapBuffers();
	for(std::vector<vertex>::iterator it = vertices.begin(); it != vertices.end(); ++it){
		*it.draw();
	}
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitwindowSize(800, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("final project");
	Init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}