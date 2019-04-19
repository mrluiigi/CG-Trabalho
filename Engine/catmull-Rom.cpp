#include "catmull-Rom.h"
#include <math.h>


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
    //Contruir matrizes P        
    float px[4] = {p0[0], p1[0], p2[0], p3[0]};
    float py[4] = {p0[1], p1[1], p2[1], p3[1]};
    float pz[4] = {p0[2], p1[2], p2[2], p3[2]};

    // A = M * P
    float ax[4];
    multMatrixVector(*m,px,ax);
    float ay[4];
    multMatrixVector(*m,py,ay);
    float az[4];
    multMatrixVector(*m,pz,az);

    // pos = T * A
    float mt[4] = {t*t*t, t*t, t,1};
    pos[0] = multVectorVector(mt,ax);
    pos[1] = multVectorVector(mt,ay);
    pos[2] = multVectorVector(mt,az);
    // deriv = T' * A
    float mtd[4] = {3*t*t, 2*t, 1,0};
    deriv[0] = multVectorVector(mtd,ax);
    deriv[1] = multVectorVector(mtd,ay);
    deriv[2] = multVectorVector(mtd,az);
}





void getGlobalCatmullRomPoint(float gt, float ** controlPoints, int controlPointsNumber,float *pos, float *deriv) {
    // Posição na curva
    float t = gt * controlPointsNumber;
    // Segmento da curva atual
    int index = floor(t); 
    // Posição no segmento
    t = t - index; // where within  the segment


    //Calcular os 4 pontos de controlo a usar
    int indices[4]; 
    indices[0] = (index + controlPointsNumber-1)%controlPointsNumber;   
    indices[1] = (indices[0]+1)%controlPointsNumber;
    indices[2] = (indices[1]+1)%controlPointsNumber; 
    indices[3] = (indices[2]+1)%controlPointsNumber;

    getCatmullRomPoint(t, controlPoints[indices[0]], controlPoints[indices[1]], controlPoints[indices[2]], controlPoints[indices[3]], pos, deriv);

}