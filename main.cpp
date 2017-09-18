#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;


std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
std::vector< glm::vec3 > temp_vertices;
std::vector< glm::vec2 > temp_uvs;
std::vector< glm::vec3 > temp_normals;

void drawScene(void);
void resize(int, int);
void keyInput(unsigned char, int, int);
void setup(void);
int readFile(char* object);

int main(int argc, char **argv)
{
    if(argv[1]==NULL)
    {
        printf("Please Specify Object File!");
    }
    else
    {
        readFile(argv[1]);
    }

    glutInit(&argc, argv);
    glutInitContextVersion(2, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ModelViewer");

    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    setup();

    glutMainLoop();
}

int readFile(char* object)
{
    char path[20];
    strcat(path,"a1files//");
    strcat(path,object);


    FILE * file = fopen(path, "r");
    if( file == NULL )
    {
        printf("Invalid File\n");
        return 1;
    }

    while( 1 )
    {

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
    }
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
