#ifdef APPLE
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <IL/il.h>

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

/** Contém todas as texturas */
vector<Texture*> textures;

/** Contém todas as luzes */
vector<Light> lights;

/** Buffers de vértices */
GLuint buffers[1];

/** Buffers de índices */
GLuint indexes[1];

/** Buffers de normais */
GLuint normals[1];

/** Buffers de texturas */
GLuint texturesBuffer[1];

//Na primeira chamada da função renderScene() vão ser carregados os buffers
bool firstRenderSceneCall = true;




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

void clearMaterials(){

    float amb[4] = {0.2,0.2,0.2,1};
    float diff[4] = {1,1,1,1};
    float spec[4] = {0,0,0,1};
    float emi[4] = {0,0,0,1};

            glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);

            glMaterialfv(GL_FRONT, GL_SPECULAR, spec);

            glMaterialfv(GL_FRONT, GL_EMISSION, emi);

            glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
        
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
    std::vector<Model> models = group.models;








    for(int i = 0; i < models.size(); i++ ){
        Model m = models[i];


        // Iniciar os vértices
        glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
        glVertexPointer(3,GL_FLOAT,0,0);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.modelInfo->numberOfVertices * 3, m.modelInfo->verticesBuffer, GL_STATIC_DRAW);

        // Iniciar os indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m.modelInfo->numberOfIndices, m.modelInfo->indicesBuffer, GL_STATIC_DRAW);

        // Iniciar as normais
        glBindBuffer(GL_ARRAY_BUFFER,normals[0]);
        glNormalPointer(GL_FLOAT,0,0);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.modelInfo->numberOfNormals * 3, m.modelInfo->normalsBuffer, GL_STATIC_DRAW);




        // Iniciar as texturas
        glBindBuffer(GL_ARRAY_BUFFER,texturesBuffer[0]);
        glTexCoordPointer(2,GL_FLOAT,0,0);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.modelInfo->numberOfTextures * 2, m.modelInfo->texturesBuffer, GL_STATIC_DRAW);


        if(m.diffuse != NULL){
            glMaterialfv(GL_FRONT, GL_DIFFUSE, m.diffuse->colour);
        }
        if(m.specular != NULL){
            glMaterialfv(GL_FRONT, GL_SPECULAR, m.specular->colour);
        }
        if(m.emissive != NULL){
            glMaterialfv(GL_FRONT, GL_EMISSION, m.emissive->colour);
        }
        if(m.ambient != NULL){
            glMaterialfv(GL_FRONT, GL_AMBIENT, m.ambient->colour);
        }






        glBindTexture(GL_TEXTURE_2D, m.texture->textureId);
        glDrawElements(GL_TRIANGLES, m.modelInfo->numberOfIndices, GL_UNSIGNED_INT, NULL);



        glBindTexture(GL_TEXTURE_2D, 0);
        
        clearMaterials();

    }


    //Desenhar subgrupos
    for(int i = 0; i < group.subGroups.size(); i++ ){
        drawGroup(group.subGroups[i]);
    }
    glPopMatrix();
}

//Carrega todos os modelo para buffers de vértice e índices
void loadBuffers() {
    glGenBuffers(1, buffers);
    glGenBuffers(1, indexes);
    glGenBuffers(1, normals);
    glGenBuffers(1, texturesBuffer);
}


void setLights() {

    GLuint luzes[8] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};


    GLfloat amb[4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat diff[4] = {1.0, 1.0, 1.0, 1.0};


    for(int i = 0; i < lights.size(); i++){

        Light l = lights[i];

        glLightfv(luzes[i], GL_DIFFUSE, diff);

        if(l.type.compare("POINT") == 0){
            GLfloat pos[4] = {l.posX, l.posY , l.posZ, 1.0};
            glLightfv(luzes[i], GL_POSITION, pos);
            glLightfv(luzes[i], GL_AMBIENT, amb);
        }

        else if(l.type.compare("DIRECTIONAL") == 0){

            GLfloat dir[4] = {l.dirX, l.dirY, l.dirZ, 0.0};
            glLightfv(luzes[i], GL_POSITION, dir);
            glLightfv(luzes[i], GL_AMBIENT, amb);
        }

        else if(l.type.compare("SPOTLIGHT") == 0){
            GLfloat pos[4] = {l.posX, l.posY, l.posZ, 1.0};
            GLfloat spotDir[3] = {l.dirX, l.dirY, l.dirZ};

            glLightfv(luzes[i], GL_POSITION, pos);
            glLightfv(luzes[i], GL_SPOT_DIRECTION, spotDir);
            glLightf(luzes[i], GL_SPOT_CUTOFF, l.cutoffAngle);
            glLightf(luzes[i], GL_SPOT_EXPONENT, l.exponent);
        }
    }
}



void loadTexture() {

    unsigned int t,tw,th;
    unsigned char *texData;
    unsigned int texID;


    for(int i = 0; i < textures.size(); i++) {
        Texture* txt = textures[i];

        string s = txt->name;

        ilInit();
        ilEnable(IL_ORIGIN_SET);
        ilOriginFunc(IL_ORIGIN_UPPER_LEFT);

        ilGenImages(1, &t);
        ilBindImage(t);
        ilLoadImage((ILstring) s.c_str());

        tw = ilGetInteger(IL_IMAGE_WIDTH);
        th = ilGetInteger(IL_IMAGE_HEIGHT);
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        texData = ilGetData();

        glGenTextures(1, &texID);

        glBindTexture(GL_TEXTURE_2D, texID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        txt->textureId = texID;
    }
}


void renderScene(void) {


    if(firstRenderSceneCall) {
        loadBuffers();
        loadTexture();
        firstRenderSceneCall = false;
    }
    glClearColor(0.0f,0.0f,0.0f,0.0f);

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // set the camera
    glLoadIdentity();

    gluLookAt(r*cos(beta)*sin(alfa), r*sin(beta),r*cos(beta)*cos(alfa),
            0.0,0.0,0.0,
            0.0f,1.0f,0.0f);


    setLights();



    glColor3f(1,0,0);


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
        loadConfig(argv[1], allModels, groups, lights, textures);


        //init GLUT and the window
        glutInit(&argc, argv);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

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


        //Light Settings
        glEnable(GL_LIGHTING);
        GLuint luzes[8] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};
        for(int i = 0; i < lights.size(); i++) {
            glEnable(luzes[i]);
        }
        glEnable(GL_TEXTURE_2D);







        //Para poder usar o glGenBuffer
        glewInit();



        //enter GLUT's main cycle
        glutMainLoop();

        return 1;
    }
}
