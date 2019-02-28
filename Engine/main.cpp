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



class Vertice{
    public:
        float x;
        float y;
        float z;
};

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

    // set the camera
    glLoadIdentity();
    gluLookAt(5.0,5.0,5.0,
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);


    // put the geometric transformations here


    // put drawing instructions here


    glEnd();
    // End of frame
    glutSwapBuffers();
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

    }

    file.close();
}

int main(int argc, char **argv) {
    lerficheiro(argv[1]);
// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1920,1080);
    glutCreateWindow("Projeto_CG");
    glClearColor(1.0,1.0,1.0,1.0);

// Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);


// put here the registration of the keyboard callbacks


//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

// enter GLUT's main cycle
    glutMainLoop();

    return 1;
}
