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

//ângulo horizontal da câmera
float alfa = 0;
//ângulo vertical da câmera
float beta = 0;
// raio / distância da câmera à origem
float r = 10;

class Vertice{
    public:
        float x;
        float y;
        float z;
};

class Model{
    public:
    	string name;
        vector<Vertice> vertices;

};

//0 -> translate
//1 -> rotate
//2 -> scale

class geometricTransforms {
public:
    int type;
    float x;
    float y;
    float z;
    float angle;
};


class Group{
public:
    vector<geometricTransforms> transforms;
    vector<Model> models;
    vector<Group> subGroups;


};


vector<Group> groups;

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
    gluLookAt(r*cos(beta)*sin(alfa), r*sin(beta),r*cos(beta)*cos(alfa),
            0.0,0.0,0.0,
            0.0f,1.0f,0.0f);

    // put the geometric transformations here


    // put drawing instructions here

    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0);
    for(int k = 0; k < groups.size(); k++) {
    	std::vector<Model> models = groups[k].models;
	    for(int i = 0; i < models.size(); i++ ){
	        Model m = models[i];

	        for(int j = 0; j < m.vertices.size(); j++) {
	            Vertice v = m.vertices[j];
	            glVertex3f(v.x, v.y, v.z);
	        }
	    }
	}

    glEnd();
    // End of frame
    glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here
    //rodar horizontalmente
    if (c == 'a') {
        alfa -= M_PI/8;
    }
    else if (c == 'd') {
        alfa += M_PI/8;
    }
    //rodar verticalmente
    else if (c == 'w' && beta < M_PI/2) {
        beta += M_PI/8;
    }
    else if (c == 's' && beta > -M_PI/2) {
        beta -= M_PI/8;
    }
    //mudar tipo de vizualização
    else if(c == '1'){
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }
    else if(c == '2'){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if(c == '3'){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glutPostRedisplay();

}

void processSpecialKeys(int key, int xx, int yy) {

    switch(key) {
        case GLUT_KEY_UP : //aproximar a câmera da origem
            if(r > 3)
                r -= 1;
            break;
        case GLUT_KEY_DOWN ://afastar a câmera da origem
            r += 1;
            break;
    }

    glutPostRedisplay();
}

//obtem um vértice a partir de uma linha do ficheiro .3d
Vertice toVertice(string s){
    Vertice v;

    float array[3];
    int i = 0;

    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) { //encontrar posição da vírgula
        token = s.substr(0, pos);   //obter substring entre virgulas
        array[i] = stof(token);     //transformar substring em float
        i++;
        s.erase(0, pos + delimiter.length());
    }
    v.x = array[0];
    v.y = array[1];
    v.z = array[2];
    return v;
}


Group parseGroup(XMLElement *group){
	Group g;
    for(XMLElement *child = group->FirstChildElement(); child != NULL; child = child->NextSiblingElement()){
        if( string(child->Name()).compare("translate") == 0) {
        	float x = atof(child->Attribute("X"));
        	float y = atof(child->Attribute("Y"));
        	float z = atof(child->Attribute("Z"));
        	geometricTransforms gt;
        	gt.type = 0;
        	gt.x = x;
        	gt.y = y;
        	gt.z = z;
        	g.transforms.push_back(gt);
        }
        else if( string(child->Name()).compare("models") == 0) {
        	    for(XMLElement *modelXML = child->FirstChildElement(); modelXML != NULL; modelXML = modelXML->NextSiblingElement()){
		        	Model m;
		        	m.name = modelXML->Attribute("file");
		        	g.models.push_back(m);
		        }
        }

    }
    return g;
}



/** Parse ao ficheiro XML */
std::vector<Group> parseXML(char* file){
    XMLDocument doc;

    std::vector<Group> res;
    //Se conseguir carregar o ficheiro
    if(!(doc.LoadFile(file))) {
        XMLElement* root = doc.FirstChildElement();
        for(XMLElement *child = root->FirstChildElement(); child != NULL; child = child->NextSiblingElement()){
            if( string(child->Name()).compare("group") == 0) {
                Group g = parseGroup(child);
                res.push_back(g);
            }
            
        }
    }
    return res;
}

Model parse3D(Model m) {
    ifstream file(m.name);
    string s;
    while(getline(file, s)){
        Vertice v = toVertice(s);
        m.vertices.push_back(v);
    }
    file.close();
    return m;

}

/** Processa os vértices lidos do ficheiro e coloca o modelo obtido no vector de modelos global*/
void lerficheiro(char* fileXML){
    groups = parseXML(fileXML);


    for (int i = 0; i < groups.size(); i++) {


    	for (int j = 0; j < groups[i].models.size(); j++) {

    		groups[i].models[j] = parse3D(groups[i].models[j]);
    	}
    }


}

void printHelp(){
    cout << "##################################################" << endl;
    cout << "#                                                #" << endl;
    cout << "# Como invocar: ./engine <XML file>              #" << endl;
    cout << "#                                                #" << endl;
    cout << "#                                                #" << endl;
    cout << "# Movimentos:                                    #" << endl;
    cout << "#          w: Move a câmara para cima            #" << endl;
    cout << "#          s: Move a câmara para baixo           #" << endl;
    cout << "#          a: Move a câmara para a esquerda      #" << endl;
    cout << "#          d: Move a câmara para a direita       #" << endl;
    cout << "#     KEY_UP: Reduz o raio da câmara             #" << endl;
    cout << "#   KEY_DOWN: Aumenta o raio da câmara           #" << endl;
    cout << "#          1: GL_POINT                           #" << endl;
    cout << "#          2: GL_LINE                            #" << endl;
    cout << "#          3: GL_FILL                            #" << endl;
    cout << "#                                                #" << endl;
    cout << "##################################################" << endl;
}


int main(int argc, char **argv) {

    if(argc < 2){
        cout << "Argumentos insuficientes" << endl;
        cout << "Faça ./engine -h para ajuda" << endl;
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
