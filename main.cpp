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

    glClearColor(0.0, 0.0, 0.0, 0.0);

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

/////////////////// vertex array

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &Indices[0]); // Vertex Array

    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);


    for(unsigned long int m = 0; m<Faces.size(); m++)
    {

        glBegin(GL_TRIANGLE_STRIP); // drawing

        for (unsigned int n=0; n<Faces[m].size(); n++)
        {
            glArrayElement(Faces[m][n]);
            //cout<<Faces[m][n];
        }

        glEnd();
    }
    glEndList();

    ///////////////////////////////////////////////////////////////////// mean

   // cout<<meanx<<" "<<meany<<" "<<meanz;

    meanx = meanx/(Indices.size()/3);
    meany = meany/(Indices.size()/3);
    meanz = meanz/(Indices.size()/3);

    cout<<meanx<<" "<<meany<<" "<<meanz;

    /////////////////////////////////////////////////////////////////////////min-max

    float translated;

    for(unsigned long int i=0; i<Indices.size(); i++)
    {

       if((i%3)==0)
       {
         translated = Indices[i]-meanx;
         if(translated<minx) {minx = translated;}
         if(translated>maxx) {maxx = translated;}
       }
       else if((i%3)==1)
       {
         translated = Indices[i]-meany;
         if(translated<miny) {miny = translated;}
         if(translated>maxy) {maxy = translated;}
       }
       else
       {
         translated = Indices[i]-meanz;
         if(translated<minz) {minz = translated;}
         if(translated>maxz) {maxz = translated;}
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


cout<<" "<<scal;

    for(unsigned long int i=0;i<Indices.size();i++)
    {
      Indices[i]=((Indices[i]/scal)*1.25); /////////////scaling factor 1.25

    }

    //cout<<"Index "<<Indices[1]<<" "<<Indices[2]<<" "<<Indices[3]<<" ";


    return 0;
}

void setup(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);


}

void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, -10.0);
    glCallList(displayList);
    glFlush();
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, 8, 100.0);
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
