#include <GL/glew.h>
#include <GL/freeglut.h>
//#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

string line;
string value;

int isOrtho = 1;

float xtrans = 0.0;
float ytrans = 0.0;
float ztrans = 0.0;

float meanx=0.0;
float meany=0.0;
float meanz=0.0;

float minx=0.0;
float miny=0.0;
float minz=0.0;

float maxx=0.0;
float maxy=0.0;
float maxz=0.0;

vector<float> Indices;
vector<vector<int> > Faces;

static unsigned int displayList; // List index.

void drawScene(void);
void resize(int, int);
void keyInput(unsigned char, int, int);
void Arrowkeys(int, int, int);
void setup(void);
int readFile(char* object); // Loader Function
void outputObject();
void Orthographic_Projection();
void Perspective_Projection();

int main(int argc, char **argv)
{

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
    glutSpecialFunc(Arrowkeys);
    if(argv[1]==NULL)
    {
        printf("\n\nPlease Specify Object File!\n\n");
    }
    else
    {
        readFile(argv[1]);
    }
    setup();

    glutMainLoop();
}

void Orthographic_Projection()
{
    cout<<"Orthographic Projection"<<"\n";
    cout.flush();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-1.0, 1.0, -1.0, 1.0, 8, 100.0); // Orthographic Projection

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();

}

void Perspective_Projection()
{
    cout<<"Perspective_Projection"<<"\n";
    cout.flush();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-1.0, 1.0, -1.0, 1.0, 8, 100.0); // Orthographic Projection

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();

}

void outputObject()
{
    cout<< "\n\n w is pressed. File writing initiated...........\n\n ";

    ofstream outObj;

    outObj.open("out.obj");

    outObj << fixed << setprecision(6);

    outObj << "v" << " " << Indices[0] << " ";

    for(unsigned long int i=1; i<Indices.size(); i++)
    {
        if((i%3)==0)
        {
            outObj << "\n" << "v" << " ";
        }

        outObj << Indices[i] << " ";

    }

    outObj << "\n";

    for(unsigned long int j=0; j<Faces.size(); j++)
    {
        outObj << "f" << " ";

        for(unsigned int k=0; k<Faces[j].size(); k++)
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
            int i = 0;

            while(iss >> Ivalue)
            {

                Indices.push_back(Ivalue);

                if(i==0)
                {
                    meanx += Ivalue;
                }
                else if(i==1)
                {
                    meany += Ivalue;
                }
                else
                {
                    meanz += Ivalue;
                }
                i++;

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
    glClearColor(0.0, 0.0, 0.0, 0.0);


/////////////////////////////////////////// mean ///////////////////////////////////////

    // cout<<meanx<<" "<<meany<<" "<<meanz;

    meanx = meanx/(Indices.size()/3);
    meany = meany/(Indices.size()/3);
    meanz = meanz/(Indices.size()/3);

    //cout<<meanx<<" "<<meany<<" "<<meanz;

    /////////////////////////////////////min-max nad traslation to the origin////////////////////////////

    float translated;

    for(unsigned long int i=0; i<Indices.size(); i++)
    {

        if((i%3)==0)
        {
            translated = Indices[i]-meanx;
            if(translated<minx)
            {
                minx = translated;
            }
            if(translated>maxx)
            {
                maxx = translated;
            }
        }
        else if((i%3)==1)
        {
            translated = Indices[i]-meany;
            if(translated<miny)
            {
                miny = translated;
            }
            if(translated>maxy)
            {
                maxy = translated;
            }
        }
        else
        {
            translated = Indices[i]-meanz;
            if(translated<minz)
            {
                minz = translated;
            }
            if(translated>maxz)
            {
                maxz = translated;
            }
        }

        Indices[i]=translated;  /////////////Index value changed



    }

//cout<<minx<<" "<< miny <<" "<< minz;
//cout<<maxx<<" "<< maxy <<" "<< maxz;
//cout<<"Index "<<Indices[1]<<" "<<Indices[2]<<" "<<Indices[3]<<" ";

////////////////////////scaling //////////////////////////////////////////

    float difx = maxx-minx;
    float dify = maxy-miny;
    float difz = maxz-minz;

    float scal = max(max(difx, dify), difz);


//cout<<" "<<scal;

    for(unsigned long int i=0; i<Indices.size(); i++)
    {
        Indices[i]=((Indices[i]/scal)*1.25); /////////////scaling factor 1.25

    }

    //cout<<"Index "<<Indices[1]<<" "<<Indices[2]<<" "<<Indices[3]<<" ";

    //////////////////////////////////// Vertex Array////////////////////////////////////////

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &Indices[0]); // Vertex Array

    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);

// Previous four line is taken from the code repository of the reference book
//https://github.com/slackmoehrle/Computer-Graphics-Through-OpenGL-2nd/blob/master/Chapter3/HelixList/helixList.cpp

    for(unsigned long int m = 0; m<Faces.size(); m++)
    {

        //glDrawElements(GL_TRIANGLE_FAN,GLsizei(Faces[m].size()),GL_INT,&Faces[m][0]);

        glBegin(GL_TRIANGLE_STRIP); // drawing

        for (unsigned int n=0; n<Faces[m].size(); n++)
        {

            glArrayElement(Faces[m][n]-1); ///// index in face value started from 1 and in vector I stored from 0;
            //cout<<Faces[m][n]<<" ";
        }
        glEnd();
    }
    glEndList();



}

void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glTranslatef(0.0, 0.0, -10.0);
    glTranslatef(xtrans, 0.0, 0.0);
    glTranslatef(0.0, ytrans, 0.0);
    glTranslatef(0.0, 0.0, ztrans);
    glCallList(displayList);
    glFlush();
    glutPostRedisplay();
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    Orthographic_Projection();
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
    case 'v':
        Orthographic_Projection();
        break;
    case 'V':
        Perspective_Projection();
        break;
    case 'n':
        ztrans -= 0.1;
        break;
    case 'N':
        ztrans += 0.1;
        break;
    default:
        break;

    }
}

void Arrowkeys(int key, int x, int y)
{
switch(key)
    {
    // Press escape to exit.
    case 27:
        exit(0);
        break;
    case GLUT_KEY_LEFT:
        xtrans -= 0.1;
        break;
    case GLUT_KEY_RIGHT:
        xtrans += 0.1;
        break;
    case GLUT_KEY_DOWN:
        ytrans -= 0.1;
        break;
    case GLUT_KEY_UP:
        ytrans += 0.1;
        break;
    default:
        break;

        glutPostRedisplay();
    }

}
