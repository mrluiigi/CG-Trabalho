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

void processSpecialKeys(int key, int xx, int yy) {

    switch(key) {

        case GLUT_KEY_LEFT :
            alfaH -= M_PI/8;
            break;
        case GLUT_KEY_RIGHT :
            alfaH += M_PI/8;
            break;
        case GLUT_KEY_UP :
            if(alfaV >= -M_PI/2)
                alfaV -= M_PI/8;
            break;
        case GLUT_KEY_DOWN :
            if(alfaV <= M_PI/2)
                alfaV += M_PI/8;
            break;
    }

    glutPostRedisplay();
}


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

/** Parse ao ficheiro XML */
std::vector<string> parseXML(char* file){
    XMLDocument doc;

    std::vector<string> res;

    //Se conseguir carregar o ficheiro
    if(!(doc.LoadFile(file))) {
        XMLElement* root = doc.FirstChildElement();

        for(XMLElement *child = root->FirstChildElement(); child != NULL; child = child->NextSiblingElement()){
            string nome = child->Attribute("file");
            //Põe no vetor
            res.push_back(nome);
            cout << nome << endl;
        }
    }
    return res;
}

/** Processa os vértices lidos do ficheiro */
void lerficheiro(char* fileXML){

    std::vector<string> v = parseXML(fileXML);

    vector<string>::iterator it;
    for(it = v.begin(); it != v.end(); it++){
        ifstream file(v[ std::distance(v.begin(), it) ]);
        string s;
        while(getline(file, s)){
            Vertice v = toVertice(s);
            vertices.push_back(v);
        }
        file.close();
    }
}

void printHelp(){
    cout << "##################################################" << endl;
    cout << "#                                                #" << endl;
    cout << "# Como invocar: ./Engine ../{XML FILE}           #" << endl;
    cout << "#                                                #" << endl;
    cout << "#                                                #" << endl;
    cout << "# Movimentos:                                    #" << endl;
    cout << "#          w: Move a câmara para cima            #" << endl;
    cout << "#          s: Move a câmara para baixo           #" << endl;
    cout << "#          a: Move a câmara para a esquerda      #" << endl;
    cout << "#          d: Move a câmara para a direita       #" << endl;
    cout << "#     KEY_UP: Move a câmara para cime            #" << endl;
    cout << "#   KEY_DOWN: Move a câmara para baixo           #" << endl;
    cout << "#   KEY_LEFT: Move a câmara para a esquerda      #" << endl;
    cout << "#  KEY_RIGHT: Move a câmara para a direita       #" << endl;
    cout << "#                                                #" << endl;
    cout << "##################################################" << endl;
}


int main(int argc, char **argv) {

    if(argc < 2){
        cout << "Argumentos insuficientes" << endl;
        cout << "Faça ./Engine -h para ajuda" << endl;
        return 0;
    }
    else if(!strcmp(argv[1], "-h")){
        printHelp();
        return 0;
    }
    else {
        lerficheiro(argv[1]);

// init GLUT and the window
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
        glutInitWindowPosition(100, 100);
        glutInitWindowSize(1920, 1080);
        glutCreateWindow("Projeto_CG");

// Required callback registry
        glutDisplayFunc(renderScene);
        glutReshapeFunc(changeSize);


// put here the registration of the keyboard callbacks
        glutKeyboardFunc(processKeys);
        glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

// enter GLUT's main cycle
        glutMainLoop();

        return 1;
    }
}
