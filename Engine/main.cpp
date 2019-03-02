#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include "tinyxml2.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace tinyxml2;
using namespace std;

float alfaH = 0;
float alfaV = 0;

class Vertice{
    public:
        float x;
        float y;
        float z;
};

std::vector<Vertice> vertices;

class Triangle{
public:
    Vertice p1;
    Vertice p2;
    Vertice p3;
};


void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}


void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // set the camera
    glLoadIdentity();
    float r = 10;
    gluLookAt(r*cos(alfaV)*sin(alfaH), r*sin(alfaV),r*cos(alfaV)*cos(alfaH),
            0.0,0.0,0.0,
            0.0f,1.0f,0.0f);


    // put the geometric transformations here


    // put drawing instructions here

    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0);

    for(int i = 0; i < vertices.size(); i++ ){
        Vertice v = vertices[i];
        glVertex3f(v.x, v.y, v.z);

    }

    glEnd();
    // End of frame
    glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

    if (c == 'a') {
        alfaH -= M_PI/8;
    }
    else if (c == 'd') {
        alfaH += M_PI/8;
    }
    else if (c == 'w' && alfaV < M_PI/2) {
        alfaV += M_PI/8;
    }
    else if (c == 's' && alfaV > -M_PI/2) {
        alfaV -= M_PI/8;
    }


    glutPostRedisplay();


}

std::vector<string> parseXML(char* file){
    XMLDocument doc;
    XMLElement * pRoot;
    std::vector<string> res;

    XMLError loaded = doc.LoadFile( file );

    if(loaded == tinyxml2::XML_SUCCESS) {
        pRoot = doc.FirstChildElement("scene");
        XMLElement * model = pRoot->FirstChildElement("model");

        const char * nome = model->Attribute("file");
        res.push_back(nome);

    }
    return res;
}

//Corrigir, não está a dar bem no final da linha
Vertice toVertice(string s){
    Vertice v;

    float array[3];
    int i = 0;

    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        array[i] = stof(token);
        i++;
        s.erase(0, pos + delimiter.length());
    }
    v.x = array[0];
    v.y = array[1];
    v.z = array[2];
    return v;
}

void lerficheiro(char* fileXML){

    std::vector<string> v = parseXML(fileXML);
    ifstream file(v[0]);

    string s;
    getline(file, s);
    while(getline(file, s)){
        Vertice v = toVertice(s);
        cout << v.x  << "," << v.y << "," << v.z << "\n";
        vertices.push_back(v);
    }

    file.close();
}

int main(int argc, char **argv) {
    lerficheiro(argv[1]);
// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("Projeto_CG");
    glutKeyboardFunc(processKeys);

// Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);


// put here the registration of the keyboard callbacks


//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

// enter GLUT's main cycle
    glutMainLoop();

    return 1;
}
