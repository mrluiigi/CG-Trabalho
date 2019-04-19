#ifdef APPLE
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "loadConfig.h"
#include "catmull-Rom.h"
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <math.h>



using namespace std;



/** Ângulo horizontal da câmera */
float alfa = 0;
/** Ângulo vertical da câmera */
float beta = 0;
/** Raio/distância da câmera à origem */
float r = 400;


/** Contém todos os modelos */
Models allModels;

/** Contém todos os grupos */
vector<Group> groups;

/** Buffers de vértices */
GLuint* buffers;

/** Buffers de índices */
GLuint* indexes;

//Na primeira chamada da função renderScene() vão ser carregados os buffers
bool firstRenderSceneCall = true;

//Variáveis globais para o cálculo da framerate
int frame=0; int timebase=0; char  s[20];


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

//desenha a curva que a translação segue
void drawTimedTranslate(TimedTranslate tt) {
    //Desenhar uma linha
    glBegin(GL_LINE_LOOP);
    float pos[3];
    float deriv[3];
    //gt controlo a posição na curva
    for (float gt = 0; gt < 1; gt += 0.01) {
        getGlobalCatmullRomPoint(gt, tt.controlPoints, tt.controlPointsNumber, pos, deriv);
        glVertex3f(pos[0],pos[1],pos[2]);
    }
    glEnd();
}



void drawGroup(Group group) {
    int  t = glutGet(GLUT_ELAPSED_TIME);
    glPushMatrix();
    //Transformações a aplicar ao grupo
    vector<GeometricTransforms> &gts = group.transforms;

    for(int i = 0; i < gts.size(); i++ ){
        GeometricTransforms gt = gts[i];

        if(gt.type == 0) {
            glTranslatef(gt.x,gt.y,gt.z);
        }
        else if(gt.type == 1) {
            glRotatef(gt.angle,gt.x,gt.y,gt.z);
        }
        else if(gt.type == 2) {
            glScalef(gt.x,gt.y,gt.z);
        }
        else if(gt.type == 3) {
            //Desenha a curva que a translação segue
            drawTimedTranslate(*gt.tt);
            //Passar tempo para milissegundos
            int time = gt.tt->time * 1000;
            //Tempo desde o inicio da volta atual
            float ct = t % time;
            //Transformar num valor entre 0 e 1
            ct = ct / time;
            float pos[3];
            float deriv[3];
            //Calcular ponto da curva
            getGlobalCatmullRomPoint(ct, gt.tt->controlPoints, gt.tt->controlPointsNumber, pos, deriv);
            glTranslatef(pos[0],pos[1],pos[2]);
        }
        else if(gt.type == 4){
            //Passar tempo para milissegundos
            int time = gt.rotateTime * 1000;
            //Tempo desde o inicio da volta atual
            float angle = t % time;
            //Transformar num valor em graus
            angle = angle / time * 360;
            glRotatef(angle,gt.x,gt.y,gt.z);
        }
    }
    //Modelos do grupo
    std::vector<int> models = group.models;

    for(int i = 0; i < models.size(); i++ ){
        Model m = allModels.getModel(models[i]);
        int id = models[i];
        //Ativar os buffers correspondentes ao modelo
        glBindBuffer(GL_ARRAY_BUFFER,buffers[id]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes[id]);
        //Desenhar os buffers
        glDrawElements(GL_TRIANGLES, m.numberOfIndices, GL_UNSIGNED_INT, NULL);
    }


    for(int i = 0; i < group.subGroups.size(); i++ ){
        drawGroup(group.subGroups[i]);
    }

    glPopMatrix();
}

//Carrega todos os modelo para buffers de vértice e índices
void loadBuffers() {
    int n = allModels.vec.size();
    buffers = new GLuint[n];
    indexes = new GLuint[n];
    glGenBuffers(n, buffers);
    glGenBuffers(n, indexes);
    //Preencher os buffers para cada modelo
    for( int i = 0; i < n; i++) {
        Model m = allModels.vec[i];
        //Tornar o buffer de vértices ativo
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,buffers[i]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //Preencher o buffer de vértices
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.numberOfVertices * 3, m.verticesBuffer, GL_STATIC_DRAW);
        //Tornar o buffer de índices ativo
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes[i]);
        //Preencher o buffer de índices
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m.numberOfIndices, m.indicesBuffer, GL_STATIC_DRAW);
    }
}

void showFps() {
    frame++;
    int time=glutGet(GLUT_ELAPSED_TIME); 
    if (time - timebase > 1000) { 
        sprintf(s,"FPS:%4.2f",
            frame*1000.0/(time-timebase)); 
        timebase = time; frame = 0; 
    }
     glutSetWindowTitle(s);  
}

void renderScene(void) {
    showFps();
    if(firstRenderSceneCall) {
        loadBuffers();
        firstRenderSceneCall = false;
    }
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // set the camera
    glLoadIdentity();
    gluLookAt(r*cos(beta)*sin(alfa), r*sin(beta),r*cos(beta)*cos(alfa),
            0.0,0.0,0.0,
            0.0f,1.0f,0.0f);

    glColor3f(1, 0, 0);
        
    for(int k = 0; k < groups.size(); k++) {
        drawGroup(groups[k]);
    }

    // End of frame
    glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here
    //rodar horizontalmente
    if (c == 'a') {
        alfa -= M_PI/48;
    }
    else if (c == 'd') {
        alfa += M_PI/48;
    }
    //rodar verticalmente
    else if (c == 'w' && beta + M_PI/48< M_PI/2) {
        beta += M_PI/48;
    }
    else if (c == 's' && beta - M_PI/48 > -M_PI/2) {
        beta -= M_PI/48;
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
        loadConfig(argv[1], allModels, groups);

        //init GLUT and the window
        glutInit(&argc, argv);

        glEnableClientState(GL_VERTEX_ARRAY);

        glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
        glutInitWindowPosition(100, 100);
        glutInitWindowSize(1920, 1080);
        glutCreateWindow("Projeto_CG");

        //Required callback registry
        glutDisplayFunc(renderScene);
        glutIdleFunc(renderScene);
        glutReshapeFunc(changeSize);


        //registration of the keyboard callbacks
        glutKeyboardFunc(processKeys);
        glutSpecialFunc(processSpecialKeys);

        //OpenGL settings
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        //Para poder usar o glGenBuffer
        glewInit();

        //enter GLUT's main cycle
        glutMainLoop();

        return 1;
    }
}
