#include "generator.h"
#include <string>
#include <math.h>


void plano(float lado, string fileName){
    ofstream file;
    file.open(fileName);

    float x, y, z;
    y = 0;
    x = lado/2;
    z = lado/2;

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
    cout << "#      -SaturnRings [RAIO1] [RAIO2] [SLICES]			        #" << endl;
    cout << "#                                                              #" << endl;
    cout << "################################################################" << endl;
}

int main(int argc, char **argv){
	
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
	}
}