#include <GL/glew.h>
#include <GL/freeglut.h>
//#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

string line;
string value;

vector<float> Indices;
vector<vector<int> > Faces;

void drawScene(void);
void resize(int, int);
void keyInput(unsigned char, int, int);
void setup(void);
int readFile(char* object); // Loader Function
void outputObject();

int main(int argc, char **argv)
{
    if(argv[1]==NULL)
    {
        printf("\n\nPlease Specify Object File!\n\n");
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

void outputObject()
{
    cout<< "\n\n w is pressed. File writing initiated...........\n\n ";

    ofstream outObj;

    outObj.open("out.obj");

    outObj << fixed << setprecision(6);

    outObj << "v" << " " << Indices[0] << " ";

    for(long int i=1; i<Indices.size(); i++)
    {
        if((i%3)==0)
        {
            outObj << "\n" << "v" << " ";
        }

        outObj << Indices[i] << " ";

    }

    outObj << "\n";

    for(long int j=0; j<Faces.size(); j++)
    {
        outObj << "f" << " ";

        for(int k=0; k<Faces[j].size(); k++)
        {
            outObj << Faces[j][k] << " ";
        }

        outObj << "\n";

    }

    outObj.close();

    cout<< "\n\nout.obj file has been created! \n\n";

}

int readFile(char* object)
{
    stringstream ss;
    ss << "a1files//" << object;
    string path = ss.str();

    ifstream file;

    file.open(path.c_str());

    if (file.is_open())
    {
        cout << "\n\nFile successfully opened !! \n\n";

    }
    else
    {
        cout << "\n\nError opening file \n\n";
    }

    cout << "\n\nFile reading started..........\n\n";

    while(getline(file,line))
    {

        istringstream iss(line);

        iss >> value;

        if(value[0] =='v') // Reading Indices
        {
            float Ivalue;

            while(iss >> Ivalue)
            {

                Indices.push_back(Ivalue);


            }
        }

        if(value[0] == 'f') // Reading Faces
        {

            int fValue;
            vector<int> temp;

            while(iss >> fValue)
            {
                temp.push_back(fValue);
            }

            Faces.push_back(temp);
            temp.clear();

        }

    }


    file.close();

    cout << "\n\nFile Reading Completed ! \n\n";

    return 0;
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
    case 'w':
        outputObject();
        break;
    default:
        break;
    }
}
