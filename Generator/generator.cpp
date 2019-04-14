#include "generator.h"
#include "loadBezierPatches.h"
#include <string>
#include <math.h>


void plano(float lado, string fileName){
    ofstream file;
    file.open(fileName);

    float x, y, z;
    y = 0;
    x = lado/2;
    z = lado/2;

    //Escreve o número de vértices na primeira linha do ficheiro
    file << 6 << endl;

    //Escreve os vértices de um triangulo no ficheiro
    file << -x << "," << y << "," << z << "," << endl; 
    file << x << "," << y << "," << z << "," << endl; 
    file << x << "," << y << "," << -z << "," << endl; 

    //Vértices do outro triângulo
    file << -x << "," << y << "," << z << "," << endl; 
    file << x << "," << y << "," << -z << "," << endl; 
    file << -x << "," << y << "," << -z << "," << endl; 


    file.close();
}


void caixa(float x, float y, float z, int divisions, string fileName){
    ofstream file;
    file.open(fileName);

    float divX = x/divisions;
    float divY = y/divisions;
    float divZ = z/divisions;

    x = x/2;
    y = y/2;
    z = z/2;

    //Escreve o número de vértices na primeira linha do ficheiro
    file << 36 * divisions * divisions << endl;

    for(int i = 0; i < divisions; i++){
        for(int j = 0; j < divisions; j++){
            //Base de baixo
            file << -x + i*divX << "," << -y << "," << -z + (j+1)*divZ << "," << endl;
            file << -x + i*divX << "," << -y << "," << -z + j*divZ << "," << endl;
            file << -x + (i+1)*divX << "," << -y << "," << -z + (j+1)*divZ << "," << endl;

            file << -x + (i+1)*divX << "," << -y << "," << -z + j*divZ << "," << endl;
            file << -x + (i+1)*divX << "," << -y << "," << -z + (j+1)*divZ << "," << endl;
            file << -x + i*divX << "," << -y << "," << -z + j*divZ << "," << endl;

            //Base de Cima
            file << -x + i*divX << "," << y << "," << -z + j*divZ << "," << endl;
            file << -x + i*divX << "," << y << "," << -z + (j+1)*divZ << "," << endl;
            file << -x + (i+1)*divX << "," << y << "," << -z + (j+1)*divZ << "," << endl;

            file << -x + (i+1)*divX << "," << y << "," << -z + (j+1)*divZ << "," << endl;
            file << -x + (i+1)*divX << "," << y << "," << -z + j*divZ << "," << endl;
            file << -x + i*divX << "," << y << "," << -z + j*divZ << "," << endl;
        
            //Face da frente
            file << -x + (i+1)*divX << "," << - y + (j+1)*divY << "," << z << "," << endl;
            file << -x + i*divX << "," << - y  + (j+1)*divY << "," << z << "," << endl;
            file << -x + (i+1)*divX << "," << -y + j*divY << "," << z << "," << endl;

            file << -x + (i+1)*divX  << "," << -y + j*divY << "," << z << "," << endl;
            file << -x + i*divX << "," << -y + (j+1)*divY << "," << z << "," << endl;
            file << -x + i*divX << "," << -y + j*divY  << "," << z << "," << endl;

            //Face de trás
            file << -x + i*divX << "," << - y  + (j+1)*divY << "," << -z << "," << endl;
            file << -x + (i+1)*divX << "," << - y + (j+1)*divY << "," << -z << "," << endl;
            file << -x + (i+1)*divX << "," << -y + j*divY << "," << -z << "," << endl;

            file << -x + i*divX << "," << -y + (j+1)*divY << "," << -z << "," << endl;
            file << -x + (i+1)*divX  << "," << -y + j*divY << "," << -z << "," << endl;
            file << -x + i*divX << "," << -y + j*divY  << "," << -z << "," << endl;

            //Face do lado esquerdo
            file << -x << "," << -y + j*divY << "," << -z + (i+1)*divZ << "," << endl;
            file << -x << "," << -y + (j+1)*divY << "," << -z + (i+1)*divZ << "," << endl;
            file << -x << "," << -y + j*divY << "," << -z + i*divZ<< "," << endl;

            file << -x << "," << -y +j*divY << "," << -z + i*divZ << "," << endl;
            file << -x << "," << -y + (j+1)*divY<< "," << -z + (i+1)*divZ << "," << endl;
            file << -x << "," << -y + (j+1)*divY<< "," << -z + i*divZ << "," << endl;

            //Face do lado direito
            file << x << "," << -y + (j+1)*divY << "," << -z + (i+1)*divZ << "," << endl;
            file << x << "," << -y + j*divY << "," << -z + (i+1)*divZ << "," << endl;
            file << x << "," << -y + j*divY << "," << -z + i*divZ<< "," << endl;

            file << x << "," << -y + (j+1)*divY<< "," << -z + (i+1)*divZ << "," << endl;
            file << x << "," << -y +j*divY << "," << -z + i*divZ << "," << endl;
            file << x << "," << -y + (j+1)*divY<< "," << -z + i*divZ << "," << endl;
        }
    }
    
    file.close();
}


void esfera(float radius, int slices, int stacks, string fileName){
    ofstream file;
    file.open(fileName);

    float alpha = (2*M_PI)/slices;
    float beta = (M_PI)/stacks;

    //Escreve o número de vértices na primeira linha do ficheiro
    file << stacks* slices * 6 << endl;


    //cada iteração desenha uma camada horizontal (do topo para baixo)
    for(int j = 0; j < stacks; j++) {

        //valores no eixo do y são constantes para cada stack
        float yUpLeft = radius * sin(M_PI/2 - beta * j);
        float yUpRight = yUpLeft;
        float yDownLeft = radius * sin(M_PI/2 - beta * (j+1));
        float yDownRight = yDownLeft;

        //Vértice no canto superior esquerdo
        float xUpLeft = radius * cos(M_PI/2 - beta * j) * sin(0 * alpha);
        float zUpLeft = radius * cos(M_PI/2 - beta * j) * cos(0 * alpha);

        //Vértice no canto inferior esquerdo
        float xDownLeft = radius * cos(M_PI/2 - beta * (j+1)) * sin(0 * alpha);     
        float zDownLeft = radius * cos(M_PI/2 - beta * (j+1)) * cos(0 * alpha);


        //cada iteração desenha uma fatia da camada horizontal
        for(int i = 0; i < slices; i++) {
            
            
            //Vértice no canto superior direito
            float xUpRight = radius * cos(M_PI/2 - beta * j) * sin((i + 1) * alpha);        
            float zUpRight = radius * cos(M_PI/2 - beta * j) * cos((i + 1) * alpha);

            
            //Vértice no canto inferior direito
            float xDownRight = radius * cos(M_PI/2 - beta * (j+1)) * sin((i + 1) * alpha);
            float zDownRight = radius * cos(M_PI/2 - beta * (j+1)) * cos((i + 1) * alpha);

            //triângulo com 2 vértices em baixo e um em cima
            file << xDownLeft << "," << yDownLeft << "," << zDownLeft << "," << endl;
            file << xDownRight << "," << yDownRight << "," << zDownRight << "," << endl;
            file << xUpLeft << "," << yUpLeft << "," << zUpLeft << "," << endl;

            //triângulo com 2 vértices em cima e um em baixo
            file << xUpRight << "," << yUpRight << "," << zUpRight << "," << endl;
            file << xUpLeft << "," << yUpLeft << "," << zUpLeft << "," << endl;
            file << xDownRight << "," << yDownRight << "," << zDownRight << "," << endl;

            //vértices da direita passam a ser os da esquerda na próxima iteração
            xUpLeft = xUpRight;
            zUpLeft = zUpRight;
            xDownLeft = xDownRight;
            zDownLeft = zDownRight; 
        }
    }
    file.close();
}


void cone(float radius, float height, int slices, int stacks, string fileName){
    ofstream file;
    file.open(fileName);

    float alpha = (2*M_PI)/slices;
    float coneSlant = sqrt(height*height+radius*radius);
    float segment = (coneSlant/stacks);
    //ângulo entre a base e a superficie lateral do cone
    float beta = atan(height/radius);

    float y = 0;
    float bx1, by1, bz1, bx2, by2, bz2, bx3, by3, bz3;
    float xDownLeft, yDownLeft, zDownLeft;
    float xDownRight, yDownRight, zDownRight;
    float xUpLeft, yUpLeft, zUpLeft;
    float xUpRight, yUpRight, zUpRight;
    //diferença entre 
    float qtdSubRadius = cos(beta) * segment;
    //difenença entra a altura de cada camada 
    float qtdAddY = sin(beta) * segment;


    //Escreve o número de vértices na primeira linha do ficheiro
    file << 3 * slices + stacks * slices * 6 << endl;

    //base do cone
    for(int i = 0; i < slices; i++) {
        bx1 = 0;
        by1 = 0;
        bz1 = 0;

        bx2 = radius * sin((i+1)*alpha);
        by2 = 0;
        bz2 = radius * cos((i+1)*alpha);

        bx3 = radius * sin(i*alpha);
        by3 = 0;
        bz3 = radius * cos(i*alpha);

        file << bx1 << "," << by1 << "," << bz1 << "," << endl;
        file << bx2 << "," << by2 << "," << bz2 << "," << endl;
        file << bx3 << "," << by3 << "," << bz3 << "," << endl;
    }
    //cada iteração desenha uma camada horizontal (da base até ao topo)
    for(int j = 0; j < stacks; j++) {
        //distância entre um dos vértices superiores e o eixo dos yy's
        float newRadius = radius - qtdSubRadius;
        //distância entre um dos vértices superiores e o plano xOz
        float newY = y + qtdAddY;
        //cada iteração desenha uma fatia da camada horizontal
        for(int i = 0; i < slices; i++) {

            //Vértice no canto inferior esquerdo
            xDownLeft = radius*sin(alpha*i);
            yDownLeft = y;
            zDownLeft = radius*cos(alpha*i);

            //Vértice no canto inferior direito
            xDownRight = radius*sin(alpha*(i+1));
            yDownRight = y;
            zDownRight = radius*cos(alpha*(i+1));

            //Vértice no canto superior esquerdo
            xUpLeft = newRadius*sin(alpha*i);
            yUpLeft = newY;
            zUpLeft = newRadius*cos(alpha*i);

            //Vértice no canto superior direito
            xUpRight = newRadius*sin(alpha*(i+1));
            yUpRight = newY;
            zUpRight = newRadius*cos(alpha*(i+1));

            //triângulo com 2 vértices em baixo e um em cima
            file << xDownLeft << "," << yDownLeft << "," << zDownLeft << "," << endl;
            file << xDownRight << "," << yDownRight << "," << zDownRight << "," << endl;
            file << xUpLeft << "," << yUpLeft << "," << zUpLeft << "," << endl;
            //triângulo com 2 vértices em cima e um em baixo
            file << xUpRight << "," << yUpRight << "," << zUpRight << "," << endl;
            file << xUpLeft << "," << yUpLeft << "," << zUpLeft << "," << endl;
            file << xDownRight << "," << yDownRight << "," << zDownRight << "," << endl;
        }

        y = newY;
        radius = newRadius;
    }

    file.close();
}


void saturnRings(float radius1, float radius2, int slices, string fileName){
    ofstream file;
    file.open(fileName);

    float bx1, by1, bz1, bx2, by2, bz2, bx3, by3, bz3;
    float bx4, by4, bz4, bx5, by5, bz5, bx6, by6, bz6;
    float alpha = (2*M_PI)/slices;

    //Escreve o número de vértices na primeira linha do ficheiro
    file << slices * 12 << endl;

    for(int i = 0; i < slices; i++) {
        bx1 = radius1 * sin((i+1)*alpha);
        by1 = 0;
        bz1 = radius1 * cos((i+1)*alpha);

        bx2 = radius2 * sin((i+1)*alpha);
        by2 = 0;
        bz2 = radius2 * cos((i+1)*alpha);

        bx3 = radius2 * sin(i*alpha);
        by3 = 0;
        bz3 = radius2 * cos(i*alpha);

        file << bx1 << "," << by1 << "," << bz1 << "," << endl;
        file << bx2 << "," << by2 << "," << bz2 << "," << endl;
        file << bx3 << "," << by3 << "," << bz3 << "," << endl;

        file << bx3 << "," << by3 << "," << bz3 << "," << endl;
        file << bx2 << "," << by2 << "," << bz2 << "," << endl;
        file << bx1 << "," << by1 << "," << bz1 << "," << endl;



        bx4 = radius1 * sin((i)*alpha);
        by4 = 0;
        bz4 = radius1 * cos((i)*alpha);

        bx5 = radius1 * sin((i+1)*alpha);
        by5 = 0;
        bz5 = radius1 * cos((i+1)*alpha);

        bx6 = radius2 * sin(i*alpha);
        by6 = 0;
        bz6 = radius2 * cos(i*alpha);

        file << bx4 << "," << by4 << "," << bz4 << "," << endl;
        file << bx5 << "," << by5 << "," << bz5 << "," << endl;
        file << bx6 << "," << by6 << "," << bz6 << "," << endl;


        file << bx6 << "," << by6 << "," << bz6 << "," << endl;
        file << bx5 << "," << by5 << "," << bz5 << "," << endl;
        file << bx4 << "," << by4 << "," << bz4 << "," << endl;
        
    }

    file.close();
}

// a matriz M é igual à transposta
float matrizM [16] = {-1,  3, -3, 1,   
                       3, -6,  3, 0,
                      -3,  3,  0, 0, 
                       1,  0,  0, 0};


void multVectorMatrix(float *v, float *m, float *res){
    //percorre vetor
    for(int i = 0; i < 4; i++){
        res[i] = 0;
        //percorre coluna matriz
        for (int j = 0; j < 4; j++){
                            //verificar se está a percorrer bem a matriz
            res[i] += v[j] * m[j * 4 + i];


            //printf("Iteração %d--%d: %f\n", i, j, res[i]);
            //printf("%f\n", v[i] * m[j * 4 + i]);
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


float calculaCoordenada(float u, float v, float *matrizPC){
    float uu[4] = {u*u*u, u*u, u, 1}; //(float) pow(u,3) _________________________________________
    float vv[4] = {v*v*v, v*v, v, 1};

    float res[4];
    float res2[4];
    float ponto = 0;

    multVectorMatrix(uu, matrizM, res);

    /*
    for (int i = 0; i < 4; ++i){
        printf("res1: %f\n", res[i]);
    }
    for (int i = 0; i < 16; ++i){
        printf("matrizPC: %f\n", matrizPC[i]);
    }*/


    multVectorMatrix(res, matrizPC, res2);

    /*
    for (int i = 0; i < 4; ++i){
        printf("res2: %f\n", res2[i]);
    }*/


    multVectorMatrix(res2, matrizM, res);
    ponto = multVectorVector(res, vv);

    return ponto;
}


float* constroiMatrizPC(BezierPatches bp, int nPatch, int coordenada){
    int* patch = bp.patches[nPatch];
    float* matrizRes = new float[16];
    int indice;

    for (int i = 0; i < 16; ++i){
        indice = patch[i];
        matrizRes[i] = bp.points[indice][coordenada];
    }

    return matrizRes;
}


float* calculaPontos(BezierPatches* bezierPatches, int nPatch, int tess, string fileName){
    ofstream file;
    file.open(fileName);
    float* temp = new float(3);
    float* matrizPCx ;
    float* matrizPCy;
    float* matrizPCz;
    float step = 1.0/tess;
    int nVerticesPatch = (tess+1)*(tess+1);

    // Número de índices
    file << tess*tess*3*2*bezierPatches->numberOfPatches << endl;
    // Índices
    for(int z = 0; z < bezierPatches->numberOfPatches; z++){
        for (int i = 0; i < tess; i++){
            for(int j = 0; j < tess; j++){
                file << nVerticesPatch*z+i*(tess+1)+j << "," << nVerticesPatch*z+i*(tess+1)+j+(tess+1) << "," << nVerticesPatch*z+i*(tess+1)+j+1 << ",";
                file << nVerticesPatch*z+i*(tess+1)+j+(tess+1) << "," << nVerticesPatch*z+i*(tess+1)+j+(tess+1)+1 << "," << nVerticesPatch*z+i*(tess+1)+j+1 << ",";
            }
        }
    }
        file << endl;

    // Número de vértices
    file << (tess+1)*(tess+1)*bezierPatches->numberOfPatches << endl;

    for(int i = 0; i < bezierPatches->numberOfPatches; i++) {

        matrizPCx = constroiMatrizPC(*bezierPatches, i, 0);
        matrizPCy = constroiMatrizPC(*bezierPatches, i, 1);
        matrizPCz = constroiMatrizPC(*bezierPatches, i, 2);



        for(float v = 0; v <= 1; v += step){
            for(float u = 0; u <= 1; u += step){
                temp[0] = calculaCoordenada(u, v, matrizPCx);
                temp[1] = calculaCoordenada(u, v, matrizPCy);
                temp[2] = calculaCoordenada(u, v, matrizPCz);
                file << temp[0] << "," << temp[1] << "," << temp[2] << "," << endl;
            }
        }
    
    }

    file.close();
}






//VIRGULAS?
void planoIndices(float lado, string fileName){
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
    


    file.close();
}







void esferaIndices(float radius, int slices, int stacks, string fileName){
    ofstream file;
    file.open(fileName);

    float alpha = (2*M_PI)/slices;
    float beta = (M_PI)/stacks;

    //Número de índices para a camada superior da esfera
    int indicesNumber = slices * 3;
    //Número de índices para cada camada da esfera (excepto as das extremidades)
    indicesNumber += slices * 3 * 2 * (stacks-2);
    //Número de índices para a camada inferior da esfera
    indicesNumber += slices * 3;

    //Vértice no topo da esfera
    int verticesNumber = 1;
    //Número de vértices das camadas das esferas 
    verticesNumber += slices * (stacks-1);
    //Vértice no fundo da esfera
    verticesNumber += 1;

    //-------------------------------------------------------INDICES---------------------------------------------------------------

    //Escreve o número de indices 
    file << indicesNumber << endl;

    //Índices da camada superior da esfera
    for (int i = 0; i < slices-1; i++){
        file << 0 << "," << i+1 << "," << i+2 << ",";
    }
    //Índices para a última slice da camada superior da esfera
    file << 0 << "," << slices << "," << 1 << ",";

    //Índice onde começam os vértices da base da stack atual
    int stackBase;
    //Índice onde começam os vértices do topo da stack atual
    int stackTop;
    int j = 0;
    //Índices das camadas da esfera (excepto as das extremidades)
    for (int i = 0; i < stacks-2; i++) {
        for (j = 0; j < slices-1; j++){
            stackTop = i * slices + 1;
            stackBase = (i+1) * slices + 1;
            //cima esquerda -> baixo esquerda -> baixo direita
            file << stackTop + j << "," << stackBase + j << "," << stackBase + j + 1 << ",";
            //cima esquerda -> baixo direita -> cima direita
            file << stackTop + j  << "," << stackBase + j + 1 << "," << stackTop + j + 1 << ",";
        }
        //Última slice de cada camada
        //cima esquerda -> baixo esquerda -> baixo direita
        file << stackTop + j << "," << stackBase + j << "," << stackBase << ",";
        //cima esquerda -> baixo direita -> cima direita
        file << stackTop + j  << "," << stackBase << "," << stackTop << ",";
    }

    //Índices da camada inferior da esfera 
    stackTop = (stacks-2) * slices + 1;
    int lastVertexIndex =  verticesNumber - 1;
    for (j = 0; j < slices-1; j++){
        file << stackTop + j << "," << lastVertexIndex << "," << stackTop + j + 1 << ",";
    }
    //Índices para a última slice da camada inferior da esfera
    file << stackTop + j << "," << lastVertexIndex << "," << stackTop << ",";

    // \n para terminar os indices
    file << endl;


    //-------------------------------------------------------VÉRTICES---------------------------------------------------------------


    //Escreve o número de vértices no ficheiro
    file << 1 + slices * (stacks-1) + 1 << endl;

    //Vértice do topo da esfera
    file << 0 << "," << radius << "," << 0 << "," << endl;

    // Vértices das camadas da esfera
    for(int j = 1; j < stacks; j++) {
        float y = radius * sin(M_PI/2 - beta * j);
        for(int i = 0; i < slices; i++) {         
            float x = radius * cos(M_PI/2 - beta * j) * sin(i * alpha);     
            float z = radius * cos(M_PI/2 - beta * j) * cos(i * alpha);
            file << x << "," << y << "," << z << "," << endl;
        }
    }

    //Vértice do fundo da esfera
    file << 0 << "," << -radius << "," << 0 << "," << endl;

    file.close();
}



void coneIndices(float radius, float height, int slices, int stacks, string fileName){
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
    int verticesNumber = 1;
    //MUDAR corpo para outra coisa!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //Número de vértices o "corpo?" do cone 
    verticesNumber += slices * stacks;
    //Vértice do topo do cone
    verticesNumber += 1;

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
    for (i = 0; i < stacks -1; i++) {
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
    int lastVertexIndex = verticesNumber - 1;
    //indices do topo do cone
    for (j = 0; j < slices-1; j++){
        file << stackBase + j + 1 << "," << lastVertexIndex << "," << stackBase + j  << ",";
    }
    //indices para "fechar" o topo do cone
    file << stackBase << "," << lastVertexIndex << "," << stackBase + j << ",";

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
    for(int i = 0; i < slices; i++) {
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
    file << 0 << "," << height << "," << 0 << "," << endl;

    file.close();
}






void saturnRingsIndice(float radius1, float radius2, int slices, string fileName){
    ofstream file;
    file.open(fileName);

    float alpha = (2*M_PI)/slices;

    float bx1, by1, bz1, bx2, by2, bz2;

    
    //Escreve os índices
    file <<  (slices-1)*6*2+4*6<< endl;

    int j;
    for (j = 0; j < slices-1; j++){   
        //baixo esquerda -> baixo direita -> cima esquerda
        file <<  j  << "," << j + 1 << "," << slices + j << ",";
        file <<  slices + j  << "," << j + 1 << "," << j << ",";
        //cima esquerda -> baixo direita -> cima direita
        file << slices + j << "," <<  j + 1 << "," << slices + j + 1 << ",";
        file << slices + j + 1 << "," <<  j + 1 << "," << slices + j << ",";
    }

    file <<  j  << "," << 0 << "," << slices + j << ",";
	file << slices + j << "," <<  0 << "," << slices << ",";
	file <<  slices + j  << "," << 0 << "," << j << ",";
	file << slices << "," <<  0 << "," << slices + j << "," << endl;


    //Escreve o número de vértices no ficheiro
    file << slices*2 << endl;

    //Vértices para cima
    for(int i = 0; i < slices; i++) {
        bx1 = radius1 * sin((i+1)*alpha);
        by1 = 0;
        bz1 = radius1 * cos((i+1)*alpha);

        file << bx1 << "," << by1 << "," << bz1 << "," << endl;
    }

    //Vértices para baixo
    for(int i = 0; i < slices; i++) {
	    bx2 = radius2 * sin((i+1)*alpha);
	    by2 = 0;
	    bz2 = radius2 * cos((i+1)*alpha);

	    file << bx2 << "," << by2 << "," << bz2 << "," << endl;
	}
        

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
    cout << "#                                                              #" << endl;
    cout << "################################################################" << endl;
}

int main(int argc, char **argv){
    BezierPatches* bezierPatches = loadBezierPatches("teapot.patch");

    float* teste = constroiMatrizPC(*bezierPatches, 0, 0);


    //calculaPontos(bezierPatches, 0, 20, "teapot.3d");

    //planoIndices(4, "plano.3d");

//    esferaIndices(8, 50, 50, "esfera.3d");

    saturnRingsIndice(15, 13, 30, "aneis.3d");


    //imprime para testar - APAGAR
    /*for (int i = 0; i < bezierPatches.numberOfPatches; i++) {
        for (int j = 0; j < 16; j++) {
            printf(" %d,", bezierPatches.patches[i][j]);
        }
        printf("\n");
    }*/

    /*for (int i = 132; i < bezierPatches->numberOfPoints; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" i:%d,j:%d,%f,",i+1,j+1, bezierPatches->points[i][j]);
        }
        printf("\n");
    }*/

    /*//imprime para testar - APAGAR
    for (int i = 0; i < bezierPatches.numberOfPoints; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" %f,", bezierPatches.points[i][j]);
        }
        printf("\n");
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////
*/
    /*
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

        else{
            cout << "Introduza os parâmetros corretamente" << endl;
            cout << "Faça ./generator -h para ajuda." << endl;
        }
        return 1;
    }*/
}