#ifdef APPLE
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <math.h>
#include "loadConfig.h"
#include <iostream>
#include <stdio.h>
#include <cstring>



using namespace std;

/** Ângulo horizontal da câmera */
float alfa = 0;
/** Ângulo vertical da câmera */
float beta = 0;
/** Raio/distância da câmera à origem */
float r = 20;


/** Contém todos os modelos */
Models allModels;

/** Contém todos os grupos */
vector<Group> groups;


/** Buffer para de desenhar (1?) */
GLuint buffers[1];

/** Buffer para os índices */
GLuint indexes[1];





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define POINT_COUNT 5
// Points that make up the loop for catmull-rom interpolation
float p[POINT_COUNT][3] = {{-1,-1,0},{-1,1,0},{1,1,0},{0,0,0},{1,-1,0}};

void buildRotMatrix(float *x, float *y, float *z, float *m) {

    m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
    m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
    m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void cross(float *a, float *b, float *res) {

    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}


void normalize(float *a) {

    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}


float length(float *v) {

    float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    return res;

}

void multMatrixVector(float *m, float *v, float *res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }

}


float multVectorVector(float *v1, float*v2){
    float res = 0;
    for (int k = 0; k < 4; ++k) {
            res += v1[k] * v2[k];
        }
}

void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv) {
    // catmull-rom matrix
    float m[4][4] = {   {-0.5f,  1.5f, -1.5f,  0.5f},
                        { 1.0f, -2.5f,  2.0f, -0.5f},
                        {-0.5f,  0.0f,  0.5f,  0.0f},
                        { 0.0f,  1.0f,  0.0f,  0.0f}};
            
    float px[4] = {p0[0], p1[0], p2[0], p3[0]};
    float py[4] = {p0[1], p1[1], p2[1], p3[1]};
    float pz[4] = {p0[2], p1[2], p2[2], p3[2]};
    // Compute A = M * P

    float ax[4];
    multMatrixVector(*m,px,ax);
    float ay[4];
    multMatrixVector(*m,py,ay);
    float az[4];
    multMatrixVector(*m,pz,az);

    // Compute pos = T * A
    float mt[4] = {t*t*t, t*t, t,1};
    pos[0] = multVectorVector(mt,ax);
    pos[1] = multVectorVector(mt,ay);
    pos[2] = multVectorVector(mt,az);
    // compute deriv = T' * A
    float mtd[4] = {3*t*t, 2*t, 1,0};
    deriv[0] = multVectorVector(mtd,ax);
    deriv[1] = multVectorVector(mtd,ay);
    deriv[2] = multVectorVector(mtd,az);
 

    // ...
}





// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float ** controlPoints, int controlPointsNumber,float *pos, float *deriv) {

    float t = gt * controlPointsNumber; // this is the real global t
    int index = floor(t);  // which segment
    t = t - index; // where within  the segment

    // indices store the points
    printf("%d\n", controlPointsNumber);
    for (int i = 0; i < controlPointsNumber; i++)
    {
        printf("%f\n", controlPoints[i][0]);
        printf("%f\n", controlPoints[i][1]);
        printf("%f\n", controlPoints[i][2]);
        printf("\n");
    }


    int indices[4]; 
    indices[0] = (index + controlPointsNumber-1)%controlPointsNumber;   
    indices[1] = (indices[0]+1)%controlPointsNumber;
    indices[2] = (indices[1]+1)%controlPointsNumber; 
    indices[3] = (indices[2]+1)%controlPointsNumber;

    getCatmullRomPoint(t, controlPoints[indices[0]], controlPoints[indices[1]], controlPoints[indices[2]], controlPoints[indices[3]], pos, deriv);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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


void drawTimedTranslate(TimedTranslate tt) {
    // draw curve using line segments with GL_LINE_LOOP

    glBegin(GL_LINE_LOOP);
    float pos[3];
    float deriv[3];

    for (float gt = 0; gt < 1; gt += 0.01) {
        getGlobalCatmullRomPoint(gt, tt.controlPoints, tt.controlPointsNumber, pos, deriv);
        glVertex3f(pos[0],pos[1],pos[2]);
    }

    glEnd();
}



void drawGroup(Group group) {


    int  t = glutGet(GLUT_ELAPSED_TIME);

    glPushMatrix();

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
            //desenha a linha que a translação segue
            drawTimedTranslate(*gt.tt);

            int time = gt.tt->time * 1000;
            float ct = t % time;
            ct = ct / time;
            float pos[3];
            float deriv[3];
            getGlobalCatmullRomPoint(ct, gt.tt->controlPoints, gt.tt->controlPointsNumber, pos, deriv);
            glTranslatef(pos[0],pos[1],pos[2]);
        }
        else if(gt.type == 4){
            int time = gt.rotateTime * 1000;
            float angle = t % time;
            angle = angle / time * 360;

            glRotatef(angle,gt.x,gt.y,gt.z);
        }
    }


    // Iniciar os buffers
    glGenBuffers(1, buffers);
    glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
    glVertexPointer(3,GL_FLOAT,0,0);

    // Iniciar os indices
    glGenBuffers(1, indexes);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes[0]);



    int inteiro = 0;
    std::vector<string> models = group.models;
    for(int i = 0; i < models.size(); i++ ){
        Model m = allModels.getModel(models[i]);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.numberOfVertices * 3, m.verticesBuffer, GL_STATIC_DRAW);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m.numberOfIndices, m.indicesBuffer, GL_STATIC_DRAW);

       /* for(; inteiro < m.numberOfVertices * 3; inteiro++){
            cout << m.verticesBuffer[inteiro] << " ";
        }
        cout << m.numberOfVertices << "\n";*/

        //glDrawArrays(GL_TRIANGLES, 0, m.numberOfVertices);

        glDrawElements(GL_TRIANGLES, m.numberOfIndices, GL_UNSIGNED_INT, NULL);
    }
    glEnd();


    for(int i = 0; i < group.subGroups.size(); i++ ){
        drawGroup(group.subGroups[i]);
        //cout << group.subGroups[i].models[0].name << "\n"; 
    }

    glPopMatrix();
}

void renderScene(void) {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    // set the camera
    glLoadIdentity();
    gluLookAt(r*cos(beta)*sin(alfa), r*sin(beta),r*cos(beta)*cos(alfa),
            0.0,0.0,0.0,
            0.0f,1.0f,0.0f);

    //drawing instructions here

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
        //aqui?
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
