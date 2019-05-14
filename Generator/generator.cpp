#include "generator.h"
#include "loadBezierPatches.h"
#include <string>
#include <math.h>



// a matriz M é igual à transposta
float mMatrix [16] = {-1,  3, -3, 1,   
                       3, -6,  3, 0,
                      -3,  3,  0, 0, 
                       1,  0,  0, 0};


void multVectorMatrix(float *v, float *m, float *res){
    //percorre vetor
    for(int i = 0; i < 4; i++){
        res[i] = 0;
        //percorre coluna matriz
        for (int j = 0; j < 4; j++){
            res[i] += v[j] * m[j * 4 + i];
        }
    }
}


float multVectorVector(float *v1, float *v2){
    float res = 0;
    for (int i = 0; i < 4; i++){
        res += v1[i] * v2[i];
    }
    return res;
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

/*
Recebe uma matriz composta por uma determinada coordenada dos pontos de controlo
Devolve a coordenada do ponto a desenhar
*/
float calculaCoordenada(float u, float v, float *controlPointsMatrix){
    float uMatrix[4] = {u*u*u, u*u, u, 1}; 
    float vMatrix[4] = {v*v*v, v*v, v, 1};

    float u_m[4];
    float u_m_p[4];
    float u_m_p_m[4];

    //B(u,v) = U * M * P * MT * V

    //U * M
    multVectorMatrix(uMatrix, mMatrix, u_m);
    //U * M * P
    multVectorMatrix(u_m, controlPointsMatrix, u_m_p);
    //U * M * P * MT
    multVectorMatrix(u_m_p, mMatrix, u_m_p_m);
    //U * M * P * MT * V
    return multVectorVector(u_m_p_m, vMatrix);
}


float calculaCoordenadaTangentesU(float u, float v, float *controlPointsMatrix){
    float vMatrix[4] = {v*v*v, v*v, v, 1};
    float uuMatrix[4] = {3*u*u, 2*u, 1, 0}; 

    float uu_m[4];
    float uu_m_p[4];
    float uu_m_p_m[4];
    float uu_m_p_m_v[4];

    //UU * M
    multVectorMatrix(uuMatrix, mMatrix, uu_m);
    //UU * M * P
    multVectorMatrix(uu_m, controlPointsMatrix, uu_m_p);
    //UU * M * P * MT
    multVectorMatrix(uu_m_p, mMatrix, uu_m_p_m);
    //UU * M * P * MT * VT
    return multVectorVector(uu_m_p_m, vMatrix);
}


float calculaCoordenadaTangentesV(float u, float v, float *controlPointsMatrix){
    float uMatrix[4] = {u*u*u, u*u, u, 1}; 
    float vvMatrix[4] = {3*v*v, 2*v, 1, 0};

    float u_m[4];
    float u_m_p[4];
    float u_m_p_m[4];
    float u_m_p_m_vv[4];

    //UU * M
    multVectorMatrix(uMatrix, mMatrix, u_m);
    //UU * M * P
    multVectorMatrix(u_m, controlPointsMatrix, u_m_p);
    //UU * M * P * MT
    multVectorMatrix(u_m_p, mMatrix, u_m_p_m);
    //UU * M * P * MT * VT
    return multVectorVector(u_m_p_m, vvMatrix);
}



/*
controlPointsMatrix corresponde a um array de 16 floats
coordenada: 
    0 -> x
    1 -> y
    2 -> z  
*/
void buildControlPointsMatrix(BezierPatches bp, int patchNumber, int coordenada, float* controlPointsMatrix){
    //obtem a localização dos índices dos pontos de controlo
    int* patch = bp.patches[patchNumber];
    int indice;

    for (int i = 0; i < 16; ++i){
        //obtem o indice de um ponto de controlo
        indice = patch[i];
        //guarda a coordenada necessária do ponto de controlo
        controlPointsMatrix[i] = bp.points[indice][coordenada];
    }

}


float* bezierPatchesGenerator(BezierPatches* bezierPatches, int tess, string fileName){
    ofstream file;
    file.open(fileName);
    float* temp = new float(3);

    float step = 1.0/tess;
    int nVerticesPatch = (tess+1)*(tess+1);
    int numberOfPatches = bezierPatches->numberOfPatches;
    //-------------------------------------------------------INDICES---------------------------------------------------------------
    //Número de índices de cada patch * número de patches
    int indicesNumber = (tess * tess * 3 * 2) * numberOfPatches;
    file << indicesNumber << endl;

    for(int z = 0; z < numberOfPatches; z++){
        int patchOffset = nVerticesPatch*z;
        for (int i = 0; i < tess; i++){
            int currentV = patchOffset + i*(tess+1);
            int nextV = currentV + tess + 1;
            for(int j = 0; j < tess; j++){
                file << currentV + j << "," << nextV + j << "," << currentV + j + 1 << ",";
                file << nextV + j << "," << nextV + j + 1 << "," << currentV + j + 1 << ",";
            }
        }
    }
        file << endl;

    //-------------------------------------------------------VÉRTICES---------------------------------------------------------------

    // Número de vértices de cada patch * 
    file << (tess+1)*(tess+1)*bezierPatches->numberOfPatches << endl;

    
    float u,v;

    float* controlPointsMatrixX = new float[16];
    float* controlPointsMatrixY = new float[16];
    float* controlPointsMatrixZ = new float[16];

    //Para cada patch
    for(int i = 0; i < numberOfPatches; i++) {
        buildControlPointsMatrix(*bezierPatches, i, 0, controlPointsMatrixX);
        buildControlPointsMatrix(*bezierPatches, i, 1, controlPointsMatrixY);
        buildControlPointsMatrix(*bezierPatches, i, 2, controlPointsMatrixZ);

        //
        v = 0;
        for(int j = 0; j <= tess; j++){
            u = 0;
            for(int k = 0; k <= tess; k++){
                temp[0] = calculaCoordenada(u, v, controlPointsMatrixX);
                temp[1] = calculaCoordenada(u, v, controlPointsMatrixY);
                temp[2] = calculaCoordenada(u, v, controlPointsMatrixZ);
                file << temp[0] << "," << temp[1] << "," << temp[2] << "," << endl;
                u += step;
            }
            v += step;
        }
    
    }

    //--------------------------------------------------------NORMAIS---------------------------------------------------------------

    //The normal vector at any point of the surface is defined as the normalized result of the cross product of the tangent vectors.

    // Número de normais de cada patch * 
    file << (tess+1)*(tess+1)*bezierPatches->numberOfPatches << endl;

    
    float m,n;

    float* tangentVectorU = new float[3];
    float* tangentVectorV = new float[3];

    //Para cada patch
    for(int i = 0; i < numberOfPatches; i++) {
        buildControlPointsMatrix(*bezierPatches, i, 0, controlPointsMatrixX);
        buildControlPointsMatrix(*bezierPatches, i, 1, controlPointsMatrixY);
        buildControlPointsMatrix(*bezierPatches, i, 2, controlPointsMatrixZ);

        //
        n = 0;
        for(int j = 0; j <= tess; j++){
            m = 0;
            for(int k = 0; k <= tess; k++){
                tangentVectorU[0] = calculaCoordenadaTangentesU(m, n, controlPointsMatrixX);
                tangentVectorU[1] = calculaCoordenadaTangentesU(m, n, controlPointsMatrixY);
                tangentVectorU[2] = calculaCoordenadaTangentesU(m, n, controlPointsMatrixZ);

                tangentVectorV[0] = calculaCoordenadaTangentesV(m, n, controlPointsMatrixX);
                tangentVectorV[1] = calculaCoordenadaTangentesV(m, n, controlPointsMatrixY);
                tangentVectorV[2] = calculaCoordenadaTangentesV(m, n, controlPointsMatrixZ);

                cross(tangentVectorV, tangentVectorU, temp);
                normalize(temp);
                

                file << temp[0] << "," << temp[1] << "," << temp[2] << "," << endl;
                m += step;
            }
            n += step;
        }
    
    }



    file.close();
}


void plano(float lado, string fileName){
    ofstream file;
    file.open(fileName);

    float x, y, z;
    y = 0;
    x = lado/2;
    z = lado/2;

    //Número de índices
    file << 6 << ", " << endl; 
    //Índices
    file << 0 << ", " << 2 << ", " << 1 << ", " << 1 << ", " << 2 << ", " << 3 <<  ", " << endl; 
    //Escreve o número de vértices na primeira linha do ficheiro
    file << 4 << ", " << endl;

    //Escreve os vértices de um triangulo no ficheiro
    file << -x << "," << y << "," << -z << "," << endl; // 0
    file << x << "," << y << "," << -z << "," << endl;  // 1
    file << -x << "," << y << "," << z << "," << endl;  // 2
    file << x << "," << y << "," << z << "," << endl;   // 3


    //--------------------------------------------------------NORMAIS---------------------------------------------------------------
    
    file << 0 << "," << 1 << "," << 0 << "," << endl;
    file << 0 << "," << 1 << "," << 0 << "," << endl;
    file << 0 << "," << 1 << "," << 0 << "," << endl;
    file << 0 << "," << 1 << "," << 0 << "," << endl;

    file.close();
}


void caixa(float x, float y, float z, int divisions, string fileName){
    ofstream file;
    file.open(fileName);

    float divX = x/divisions;
    float divY = y/divisions;
    float divZ = z/divisions;


    //-------------------------------------------------------INDICES---------------------------------------------------------------


    //Número de indices
    file << 6 * (divisions * divisions) * 3 * 2 << endl;


    int currentLine, nextLine, currentFace;
    //Escreve os índices
    for(int q = 0; q < 6; q++){
        currentFace = q * (divisions+1) * (divisions+1);
        for(int h = 0; h < divisions; h++){
            currentLine = (divisions+1)*h + currentFace;
            nextLine = (divisions+1)*(h+1) + currentFace;

            for(int g = 0;  g < divisions ; g++){
                file << currentLine + g << "," << nextLine + g + 1 <<  "," << currentLine + g + 1 << ",";
                file << nextLine + g << "," << nextLine + g + 1 << "," << currentLine + g << ",";
            }
        }
    }


    file << endl;


    //-------------------------------------------------------VÉRTICES---------------------------------------------------------------


    //Número de vértices
    file << 6 * ( (divisions+1) * (divisions+1) ) << endl;


    //Top
    for(int j = 0; j <= divisions; j++){
        for(int i = 0; i <= divisions; i++){
            file << -x/2 + divX*i << "," << y/2 << "," << -z/2 + divZ*j << "," << endl;
        }
    }


    //Left
    for(int j = 0; j <= divisions; j++){
        for(int i = 0; i <= divisions; i++){
            file << -x/2 << "," << y/2 - divY*j << "," << -z/2 + divZ*i << "," << endl;
        }
    }


    //Front
    for(int j = 0; j <= divisions; j++){
        for(int i = 0; i <= divisions; i++){
            file << -x/2 + divX*i << "," << y/2 - divY*j << "," << z/2 << "," << endl;
        }
    }


    //Right
    for(int j = 0; j <= divisions; j++){
        for(int i = 0; i <= divisions; i++){
            file << x/2 << "," << y/2 - divY*j << "," << z/2 - divZ*i << "," << endl;
        }
    }


    //Back
    for(int j = 0; j <= divisions; j++){
        for(int i = 0; i <= divisions; i++){
            file << x/2 - divX*i << "," << y/2 - divY*j << "," << -z/2 << "," << endl;
        }
    }


    //Bottom
    for(int j = 0; j <= divisions; j++){
        for(int i = 0; i <= divisions; i++){
            file << -x/2 + divX*i << "," << -y/2 << "," << z/2 - divZ*j << "," << endl;
        }
    }


    //--------------------------------------------------------NORMAIS---------------------------------------------------------------


    //Top
    for(int i = 0; i <= divisions; i++){
        for(int j = 0; j <= divisions; j++){
            file << 0 << "," << 1 << "," << 0 << "," << endl;
        }
    }


    //Left
    for(int i = 0; i <= divisions; i++){
        for(int j = 0; j <= divisions; j++){
            file << -1 << "," << 0 << "," << 0 << "," << endl;            
        }
    }


    //Front
    for(int i = 0; i <= divisions; i++){
        for(int j = 0; j <= divisions; j++){
            file << 0 << "," << 0 << "," << 1 << "," << endl;
        }
    }


    //Right
    for(int i = 0; i <= divisions; i++){
        for(int j = 0; j <= divisions; j++){
            file << 1 << "," << 0 << "," << 0 << "," << endl;
        }
    }


    //Back
    for(int i = 0; i <= divisions; i++){
        for(int j = 0; j <= divisions; j++){
            file << 0 << "," << 0 << "," << -1 << "," << endl;
        }
    }


    //Bottom
    for(int i = 0; i <= divisions; i++){
        for(int j = 0; j <= divisions; j++){
            file << 0 << "," << -1 << "," << 0 << "," << endl;
        }
    }


    //--------------------------------------------------------TEXTURA---------------------------------------------------------------


    //Top
    for(int i = 0; i <= divisions; i++){
        for(int j = 0; j <= divisions; j++){
            file << .25f + j*(.25/divisions) << "," << i*(1.0f/3.0f)/divisions << "," << endl;
        }
    }

    //Left
    for(int i = 0; i <= divisions; i++){
        for(int j = 0; j <= divisions; j++){
            file << j*(.25/divisions) << "," << 1.0f/3.0f + i*(1.0f/3.0f)/divisions << "," << endl;
        }
    }

    //Front
    for(int i = 0; i <= divisions; i++){
        for(int j = 0; j <= divisions; j++){
            file << .25f + j*(.25/divisions) << "," << 1.0f/3.0f + i*(1.0f/3.0f)/divisions << "," << endl;
        }
    }

    //Right
    for(int i = 0; i <= divisions; i++){
        for(int j = 0; j <= divisions; j++){
            file << .5f + j*(.25/divisions) << "," << 1.0f/3.0f + i*(1.0f/3.0f)/divisions << "," << endl;
        }
    }

    //Back
    for(int i = 0; i <= divisions; i++){
        for(int j = 0; j <= divisions; j++){
            file << .75f + j*(.25/divisions) << "," << 1.0f/3.0f + i*(1.0f/3.0f)/divisions << "," << endl;
        }
    }

    //Bottom
    for(int i = 0; i <= divisions; i++){
        for(int j = 0; j <= divisions; j++){
            file << .25f + j*(.25/divisions) << "," << 2.0f/3.0f + i*(1.0f/3.0f)/divisions << "," << endl;
        }
    }


    file.close();
}


void esfera(float radius, int slices, int stacks, string fileName){
    ofstream file;
    file.open(fileName);

    float alpha = (2*M_PI)/slices;
    float beta = (M_PI)/stacks;
    float s = 0;
    float t = 0;

    //Número de índices para a camada superior da esfera
    int indicesNumber = slices * 3;
    //Número de índices para cada camada da esfera (excepto as das extremidades)
    indicesNumber += slices * 3 * 2 * (stacks-2);
    //Número de índices para a camada inferior da esfera
    indicesNumber += slices * 3;

    //Vértice no topo da esfera
    int verticesNumber = slices;
    //Número de vértices das camadas das esferas 
    verticesNumber += (slices+1) * (stacks-1);
    //Vértice no fundo da esfera
    verticesNumber += slices;

    //-------------------------------------------------------INDICES---------------------------------------------------------------

    //Escreve o número de indices 
    file << indicesNumber << endl;
    int topVertexOffset = slices;
    //Índices da camada superior da esfera
    for (int i = 0; i < slices; i++){
        file << i << "," << i+topVertexOffset << "," << (i+1)+topVertexOffset << ",";
    }
    //Índices para a última slice da camada superior da esfera
    //file << 0 << "," << slices << "," << 1 << ",";

    //Índice onde começam os vértices da base da stack atual
    int stackBase;
    //Índice onde começam os vértices do topo da stack atual
    int stackTop;
    int j = 0;
    //Índices das camadas da esfera (excepto as das extremidades)
    for (int i = 0; i < stacks-2; i++) {
        for (j = 0; j < slices; j++){
            stackTop = i * (slices+1) + topVertexOffset;
            stackBase = (i+1) * (slices+1) + topVertexOffset;
            //cima esquerda -> baixo esquerda -> baixo direita
            file << stackTop + j << "," << stackBase + j << "," << stackBase + j + 1 << ",";
            //cima esquerda -> baixo direita -> cima direita
            file << stackTop + j  << "," << stackBase + j + 1 << "," << stackTop + j + 1 << ",";
        }
        //Última slice de cada camada
        //cima esquerda -> baixo esquerda -> baixo direita
        //file << stackTop + j << "," << stackBase + j << "," << stackBase << ",";

        //cima esquerda -> baixo direita -> cima direita
        //file << stackTop + j  << "," << stackBase << "," << stackTop << ",";
    }


    //Índices da camada inferior da esfera 
    stackTop = (stacks-2) * (slices+1) + topVertexOffset;
    int bottomStackOffset =  verticesNumber - slices;
    for (int j = 0; j < slices; j++){
        file << stackTop + j << "," << bottomStackOffset + j << "," << stackTop + j + 1 << ",";
    }
    //Índices para a última slice da camada inferior da esfera
    //file << stackTop + j << "," << lastVertexIndex << "," << stackTop << ",";






    // \n para terminar os indices
    file << endl;


    //-------------------------------------------------------VÉRTICES---------------------------------------------------------------
    float x = 0;
    float z = 0;

    //Escreve o número de vértices no ficheiro
    file << verticesNumber << endl;

    //Vértice do topo da esfera
    for(int i = 0; i < slices; i++){
        file << 0 << "," << radius << "," << 0 << "," << endl;
    }

    // Vértices das camadas da esfera
    for(int j = 1; j < stacks; j++) {
        float y = radius * sin(M_PI/2 - beta * j);
        for(int i = 0; i < slices+1; i++) {         
            x = radius * cos(M_PI/2 - beta * j) * sin(i * alpha);     
            z = radius * cos(M_PI/2 - beta * j) * cos(i * alpha);
            file << x << "," << y << "," << z << "," << endl;
        }
    }

    //Vértice do fundo da esfera
    for(int i = 0; i < slices; i++){
        file << 0 << "," << -radius << "," << 0 << "," << endl;
    }

    //--------------------------------------------------------NORMAIS---------------------------------------------------------------

    //Vetor normal para topo da esfera
    for(int i = 0; i < slices; i++){
        file << 0 << "," << 1 << "," << 0 << "," << endl;
    }

    for(int j = 1; j < stacks; j++) {
        float y = sin(M_PI/2 - beta * j);
        for(int i = 0; i < slices+1; i++) {         
            x = cos(M_PI/2 - beta * j) * sin(i * alpha);     
            z = cos(M_PI/2 - beta * j) * cos(i * alpha);
            file << x << "," << y << "," << z << "," << endl;
        }
    }

    //Vetor normal para fundo da esfera da esfera
    for(int i = 0; i < slices; i++){
        file << 0 << "," << -1 << "," << 0 << "," << endl;
    }

    //--------------------------------------------------------TEXTURA---------------------------------------------------------------

    //número de coordenadas da textura igual a número de vértices e normais?
    float offset = (1.0f/slices)/2.0f;

    for(int i = 0; i < slices; i++){
        s = (float)(i+offset) / slices;
        t = 0;
        file << s << "," << t << "," << endl;
    }

    for(int j = 1; j < stacks; j++) {
        for(int i = 0; i < slices+1; i++) { 
            s = (float)i / slices;
            t = (float)j / stacks;
            file << s << "," << t << "," << endl;
        }
    }

    for(int i = 0; i < slices; i++){
        s = (float)(i+offset) / slices;
        t = 1;
        file << s << "," << t << "," << endl;
    }

    file.close();
}


void cone(float radius, float height, int slices, int stacks, string fileName){
    ofstream file;
    file.open(fileName);

    //Número de índices para a base do cone
    int indicesNumber = slices * 3;
    //Número de índices para cada stack do cone (excepto a última)
    indicesNumber += slices * (stacks -1) * 3 * 2;
    //Número de índices para o último stack do cone 
    indicesNumber += slices * 3;
    file << indicesNumber << endl;

    //Vértice do centro da base do cone
    int verticesNumber = 1+slices;
    //MUDAR corpo para outra coisa!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //Número de vértices o "corpo?" do cone 
    verticesNumber += slices * stacks;
    //Vértice do topo do cone
    verticesNumber += slices;

    //-------------------------------------------------------INDICES---------------------------------------------------------------

    //Índices da base do cone
    for (int i = 0; i < slices-1; i++){
        file << i+1 << "," << 0 << "," << i+2 << ",";
    }
    //Índices da última slice da base do cone
    file << slices << "," << 0 << "," << 1 << ",";

    //Índice onde começam os vértices da base da stack atual
    int stackBase;
    //Índice onde começam os vértices do topo da stack atual
    int stackTop;
    int j = 0;
    int i = 0;
    //Índices para cada stack do cone (excepto o último)
    for (i = 1; i < stacks; i++) {
        for (j = 0; j < slices-1; j++){
            stackBase = i * slices + 1;
            stackTop = (i+1) * slices + 1;
            //baixo esquerda -> baixo direita -> cima esquerda
            file << stackBase + j  << "," << stackBase + j + 1 << "," << stackTop + j << ",";
            //cima esquerda -> baixo direita -> cima direita
            file << stackTop + j << "," << stackBase + j + 1 << "," << stackTop + j + 1 << ",";
        }
        //Última slice do stack
        //baixo esquerda -> baixo direita -> cima esquerda
        file << stackBase + j  << "," << stackBase << "," << stackTop + j << ",";
        //cima esquerda -> baixo direita -> cima direita
        file << stackTop + j << "," << stackBase << "," << stackTop << ",";
    }

    stackBase = i * slices + 1;
    int lastVertexIndex = verticesNumber - slices;
    //indices do topo do cone
    for (j = 0; j < slices-1; j++){
        file << stackBase + j + 1 << "," << lastVertexIndex + j << "," << stackBase + j  << ",";
    }
    //indices para "fechar" o topo do cone
    file << stackBase << "," << lastVertexIndex + j << "," << stackBase + j << ",";

    // \n para terminar os indices
    file << endl;

    //-------------------------------------------------------VÉRTICES---------------------------------------------------------------

    float alpha = (2*M_PI)/slices;
    float coneSlant = sqrt(height*height+radius*radius);
    float segment = (coneSlant/stacks);
    //ângulo entre a base e a superficie lateral do cone
    float beta = atan(height/radius);
    //diferença entre 
    float qtdSubRadius = cos(beta) * segment;
    //difenença entra a altura de cada camada 
    float qtdAddY = sin(beta) * segment;

    file << verticesNumber<< endl;

    //Vértice no centro da base
    file << 0 << "," << 0 << "," << 0 << "," << endl;

    //Vértices da borda da base do cone
    for(int i = 0; i < slices*2; i++) {
        file << radius * sin(i*alpha) << "," << 0 << "," << radius * cos(i*alpha) << "," << endl;
    }

    float y = 0;
    for(int j = 1; j < stacks ; j++) {
        //distância entre um dos vértices superiores e o eixo dos yy's
        float newRadius = radius - qtdSubRadius;
        //distância entre um dos vértices superiores e o plano xOz
        float newY = y + qtdAddY;
        for(int i = 0; i < slices; i++) {
            file << newRadius*sin(alpha*i) << "," << newY << "," << newRadius*cos(alpha*i) << "," << endl;
        }
        y = newY;
        radius = newRadius;
    }

    //Vértice no topo do cone
    for(int i = 0; i < slices; i++){
        file << 0 << "," << height << "," << 0 << "," << endl;
    }

    //--------------------------------------------------------NORMAIS---------------------------------------------------------------

    //Normal no fundo
    for(int z = 0; z < (slices+1); z++){
        file << 0 << "," << -1 << "," << 0 << "," << endl;
    }

    //Normais da superfície lateral
    for(int i = 0; i < stacks+1; i++){
        for(int j = 0; j < slices; j++){
            file << sin(alpha*j) << "," << sin(90-beta) << "," << cos(alpha*j) << endl;
        }
    }
    
    //Normais do topo
    for(int j = 0; j < slices; j++){
        file << sin(alpha*j) << "," << sin(90-beta) << "," << cos(alpha*j) << endl;
    }


    file.close();
}


void saturnRings(float radius1, float radius2, int slices, string fileName){
    ofstream file;
    file.open(fileName);

    float alpha = (2*M_PI)/slices;

    float bx1, by1, bz1, bx2, by2, bz2;

    float s = 0;
    float t = 0;

    
    //Escreve os índices
    file << 3*2*(slices+1) << endl;

    int j;
    for (j = 0; j < slices+1; j++){   
        file <<  (slices+1) + j  << "," << j + 1 << "," << j << ",";
        file << (slices+1) + j + 1 << "," <<  j + 1 << "," << (slices + 1) + j << ",";
    }
    file << endl;


    //--------------------------------------------------------VÉRTICES---------------------------------------------------------------

    //Escreve o número de vértices no ficheiro
    file << (slices+1)*2 << endl;

    //Vértices parte de fora do anel
    for(int i = 0; i < slices+1; i++) {
        bx1 = radius1 * sin((i+1)*alpha);
        by1 = 0;
        bz1 = radius1 * cos((i+1)*alpha);

        file << bx1 << "," << by1 << "," << bz1 << "," << endl;
    }

    //Vértices parte de dentro do anel
    for(int i = 0; i < slices+1; i++) {
	    bx2 = radius2 * sin((i+1)*alpha);
	    by2 = 0;
	    bz2 = radius2 * cos((i+1)*alpha);

	    file << bx2 << "," << by2 << "," << bz2 << "," << endl;
	}

    //--------------------------------------------------------NORMAIS---------------------------------------------------------------
    
    //Normais
    for(int z = 0; z < ((slices+1)*2); z++) {
        file << 0 << "," << 1 << "," << 0 << "," << endl;
    }

    //--------------------------------------------------------TEXTURAS---------------------------------------------------------------

   
    for(int i = 0; i < slices; i++){
        s = (float)i / slices;
        t = 0;
        file << s << "," << t << "," << endl;
    }
    file << 1 << "," << 0 << "," << endl;

    for(int i = 0; i < slices; i++){
        s = (float)i / slices;
        t = 1;
        file << s << "," << t << "," << endl;
    }
    file << 1 << "," << 1 << "," << endl;
    


    file.close();
}


void printHelp(){
    cout << "################################################################" << endl;
    cout << "#                                                              #" << endl;
    cout << "# Como invocar:                                                #" << endl;
    cout << "#      ./generator {COMANDO} {FICHEIRO}                        #" << endl;
    cout << "#                                                              #" << endl;
    cout << "#                                                              #" << endl;
    cout << "# Comandos:                                                    #" << endl;
    cout << "#      -Plano [Tamanho]                                        #" << endl;
    cout << "#      -Caixa [Tamanho X] [Tamanho Y] [Tamanho Z] [DIVISÕES]   #" << endl;
    cout << "#      -Esfera [RAIO] [SLICES] [STACKS]                        #" << endl;
    cout << "#      -Cone [RAIO] [ALTURA] [SLICES] [STACKS]                 #" << endl;
    cout << "#      -SaturnRings [RAIO1] [RAIO2] [SLICES]                   #" << endl;
    cout << "#      -BezierPatch [PATCH] [TESSELACAO]                       #" << endl;
    cout << "#                                                              #" << endl;
    cout << "################################################################" << endl;
}

int main(int argc, char **argv){

    //saturnRingsIndice(7, 11, 20, "saturnRings1.3d");
    //saturnRingsIndice(12, 14, 20, "saturnRings2.3d");
    //esferaIndices(5, 10, 20, "esfera.3d");
    //esferaIndices(5, 20, 30, "sol.3d");

    //bezierPatchesGenerator(bezierPatches, 0, 20, "teapot.3d");

    //planoIndices(4, "plano.3d");

    //esferaIndices(8, 50, 50, "esfera.3d");

    //saturnRingsIndice(15, 13, 30, "aneis.3d");

    //caixaIndices(6, 6, 6, 16, "caixa.3d");

    
    if (argc < 2){
        cout << "Parâmetros insuficientes" << endl;
        cout << "Faça ./generator -h para ajuda." << endl;
        return -1;
    } 
    else if(!strcmp(argv[1], "-h")){
        printHelp();
        return 0;
    }
    else if(argc > 1){
        string file = argv[argc-1];
        //Plano: lado
        if(strcmp(argv[1], "Plano") == 0 && argc == 4){
            plano(atof(argv[2]), file);
        }
        //Plano: lado x, lado y, lado z, número de divisões(opcional)
        else if(strcmp(argv[1], "Caixa") == 0 && (argc == 6 || argc == 7)){ 
            if(argc == 6){
                caixa(atof(argv[2]), atof(argv[3]), atof(argv[4]), 1, file);
            }
            else
                caixa(atof(argv[2]), atof(argv[3]), atof(argv[4]), atoi(argv[5]), file);
        }
        //Esfera: raio, slices,stacks
        else if(strcmp(argv[1], "Esfera") == 0 && argc == 6){
            esfera(atof(argv[2]), atoi(argv[3]), atoi(argv[4]), file);
        }
        //raio, altura, slices, stacks,
        else if(strcmp(argv[1], "Cone") == 0 && argc == 7){
            cone(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), file);
        }
        //raio1, raio2, slices
        else if(strcmp(argv[1], "SaturnRings") == 0 && argc == 6){
            saturnRings(atof(argv[2]), atof(argv[3]), atoi(argv[4]), file);
        }
        else if(strcmp(argv[1], "BezierPatch") == 0 && argc == 5){
            BezierPatches* bezierPatches = loadBezierPatches(argv[2]);
            bezierPatchesGenerator(bezierPatches, atof(argv[3]), file);
        }


        else{
            cout << "Introduza os parâmetros corretamente" << endl;
            cout << "Faça ./generator -h para ajuda." << endl;
        }
        return 1;
    }
}