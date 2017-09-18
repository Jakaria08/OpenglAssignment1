#include <GL/glew.h>
#include <GL/freeglut.h>

void drawScene(void);
void resize(int, int);
void keyInput(unsigned char, int, int);
void setup(void);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(2, 1);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("square.cpp");

	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	setup();

	glutMainLoop();
}

void setup(void) 
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex3f(20.0, 20.0, 0.0);
		glVertex3f(80.0, 20.0, 0.0);
		glVertex3f(80.0, 80.0, 0.0);
		glVertex3f(20.0, 80.0, 0.0);
	glEnd();
	glFlush(); 
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyInput(unsigned char key, int x, int y)
{
	switch(key) 
	{
	  // Press escape to exit.
		case 27:
			exit(0);
			break;
		default:
			break;
	}
}
