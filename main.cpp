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

int Ortho=1;
int Persp=0;

float xtrans = 0.0;
float ytrans = 0.0;
float ztrans = 0.0;

int xrot = 0;
int yrot = 0;
int zrot = 0;

float xtransCam = 0.0;
float ytransCam = 0.0;
float ztransCam = 0.0;

int xrotCam = 0;
int yrotCam = 0;
int zrotCam = 0;


float meanx=0.0;
float meany=0.0;
float meanz=0.0;

float minx=0.0;
float miny=0.0;
float minz=0.0;

float maxx=0.0;
float maxy=0.0;
float maxz=0.0;


GLfloat modelMatrix[16];

vector<float> Indices;
vector<vector<int> > Faces;


static int fogMode = GL_LINEAR; // Fog mode.
static float fogStart = 10.0; // Fog start z value.
static float fogEnd = 20.0; // Fog end z value.
static float RotModel[16];

// taken from the fog model of the reference textbook

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
void RotationMX(int angle);
void RotationMY(int angle);
void RotationMZ(int angle);


int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitContextVersion(2, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
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


void RotationMX(int angle)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(RotModel);
    glRotatef(angle,1,0,0);
    glGetFloatv(GL_MODELVIEW_MATRIX, RotModel);
    glPopMatrix();

    // get the idea from this link: http://in.somniac.me/2010/03/using-opengl-to-calculate-transformations/
}

void RotationMY(int angle)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(RotModel);
    glRotatef(angle,0,1,0);
    glGetFloatv(GL_MODELVIEW_MATRIX, RotModel);
    glPopMatrix();
}

void RotationMZ(int angle)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(RotModel);
    glRotatef(angle,0,0,1);
    glGetFloatv(GL_MODELVIEW_MATRIX, RotModel);
    glPopMatrix();
}

void TranslationMX(float transMX)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(RotModel);
    glTranslatef(transMX,0,0);
    glGetFloatv(GL_MODELVIEW_MATRIX, RotModel);
    glPopMatrix();
}

void TranslationMY(float transMY)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(RotModel);
    glTranslatef(0,transMY,0);
    glGetFloatv(GL_MODELVIEW_MATRIX, RotModel);
    glPopMatrix();
}

void TranslationMZ(float transMZ)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(RotModel);
    glTranslatef(0,0,transMZ);
    glGetFloatv(GL_MODELVIEW_MATRIX, RotModel);
    glPopMatrix();
}

void Orthographic_Projection()
{
    cout<<"Orthographic Projection"<<"\n";
    cout.flush();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-1.0, 1.0, -1.0, 1.0, 8, 100.0); // Orthographic Projection

    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

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
    //glLoadIdentity();
    //glTranslatef(xtransCam, ytransCam, ztransCam);
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


    //Initial setup for Model Rotation

    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0.0, 0.0, -10.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, RotModel);
    glLoadIdentity();





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

        glBegin(GL_TRIANGLE_FAN); // drawing

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
    glEnable(GL_DEPTH_TEST);

    float fogColor[4] = {0.0, 0.0, 0.0, 0.5};

    //////////////////// Fog ////////////////////////
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, fogMode);
    glFogf(GL_FOG_START, fogStart);
    glFogf(GL_FOG_END, fogEnd);
    glHint(GL_FOG_HINT, GL_NICEST);
    glDisable(GL_FOG);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glTranslatef(xtransCam, 0.0, 0.0);
    glTranslatef(0.0, ytransCam, 0.0);
    glTranslatef(0.0, 0.0, ztransCam);


    glRotatef(xrotCam,1.0,0.0,0.0);
    glRotatef(yrotCam,0.0,1.0,0.0);
    glRotatef(zrotCam,0.0,0.0,1.0);
    // glTranslatef(0,0,-1);
    gluLookAt(0, 0, 1.0, 0, 0, 0, 0, 1, 0);
    glMultMatrixf(RotModel);

    //glTranslatef(0.0, 0.0, -10.0);


    //glTranslatef(xtransCam, ytransCam, ztransCam);



    //glTranslatef(xtrans, 0.0, 0.0);
    //glTranslatef(0.0, ytrans, 0.0);
    //glTranslatef(0.0, 0.0, ztrans);
    // glRotatef(xrot,1.0,0.0,0.0);
    // glRotatef(yrot,0.0,1.0,0.0);
    //  glRotatef(zrot,0.0,0.0,1.0);

    glCallList(displayList);

    //glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();
    //glLoadIdentity();

// transformations go here




    // glGetFloatv (GL_MODELVIEW, modelMatrix);
    //glMultMatrixf(modelMatrix);
    //glPopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);

    if(Ortho == 1)
        Orthographic_Projection();

    else
        Perspective_Projection();
}


void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
    // Press escape to exit.
    case 'q':
        exit(0);
        break;
    case 'w':
        outputObject();
        break;
    case 'v':
        Ortho = 1;
        Persp = 0;
        Orthographic_Projection();
        break;
    case 'V':
        Persp =1;
        Ortho =0;
        Perspective_Projection();
        break;
    case 'n':
        ztrans -= 0.1;
        TranslationMZ(-0.1);
        break;
    case 'N':
        ztrans += 0.1;
        TranslationMZ(0.1);
        break;
    case 'p':
        xrot -= 10;
        RotationMX(-10);
        break;
    case 'P':
        xrot += 10;
        RotationMX(10);
        break;
    case 'y':
        yrot -= 10;
        RotationMY(-10);
        break;
    case 'Y':
        yrot += 10;
        RotationMY(10);
        break;
    case 'r':
        zrot -= 10;
        RotationMZ(-10);
        break;
    case 'R':
        zrot += 10;
        RotationMZ(10);
        break;
    case 'd':
        xtransCam -= 0.1;
        break;
    case 'D':
        xtransCam += 0.1;
        break;
    case 'c':
        ytransCam -= 0.1;
        break;
    case 'C':
        ytransCam += 0.1;
        break;
    case 'z':
        ztransCam -= 0.1;
        break;
    case 'Z':
        ztransCam += 0.1;
        break;
    case 't':
        xrotCam -= 1;
        break;
    case 'T':
        xrotCam += 1;
        break;
    case 'a':
        yrotCam -= 1;
        break;
    case 'A':
        yrotCam += 1;
        break;
    case 'l':
        zrotCam -= 1;
        break;
    case 'L':
        zrotCam += 1;
        break;
    case 'f':
        glDisable(GL_FOG);
        break;
    case 'x':
        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0, 0.0, -10.0);
        glGetFloatv(GL_MODELVIEW_MATRIX, RotModel);
        glPopMatrix();
        xtransCam=ytransCam=ztransCam=xrotCam=yrotCam=zrotCam=0.0;
        break;
    default:
        break;

        glutPostRedisplay();

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
        TranslationMX(-0.1);
        break;
    case GLUT_KEY_RIGHT:
        xtrans += 0.1;
        TranslationMX(0.1);
        break;
    case GLUT_KEY_DOWN:
        ytrans -= 0.1;
        TranslationMY(-0.1);
        break;
    case GLUT_KEY_UP:
        ytrans += 0.1;
        TranslationMY(0.1);
        break;
    default:
        break;

        glutPostRedisplay();
    }

}
