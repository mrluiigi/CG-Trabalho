#include "generator.h"
#include <string>
#include <math.h>


void plano(int lado, string fileName){
    ofstream file;
    file.open(fileName);

    float x, y, z;
    y = 0;
    x = lado/2;
    z = lado/2;

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


void caixa(float x, float y, float z, string fileName){
	ofstream file;
	file.open(fileName);

	y = y/2;
	x = x/2;
	z = z/2;

	file << 36 << endl;

	//base de baixo
    file << x << "," << -y << "," << z << "," << endl;
    file << -x << "," << -y << "," << -z << "," << endl;
    file << x << "," << -y << "," << -z << "," << endl;
    
    file << -x << "," << -y << "," << -z << "," << endl;
    file << x << "," << -y << "," << z << "," << endl;
    file << -x << "," << -y << "," << z << "," << endl;

    //base de cima
    file << x << "," << y << "," << z << "," << endl;
    file << x << "," << y << "," << -z << "," << endl;
    file << -x << "," << y << "," << -z << "," << endl;

    file << -x << "," << y << "," << -z << "," << endl;
    file << -x << "," << y << "," << z << "," << endl;
    file << x << "," << y << "," << z << "," << endl;
    
    //face 4
    file << x << "," << -y << "," << -z << "," << endl;
    file << x << "," << y << "," << z << "," << endl;
    file << x << "," << -y << "," << z << "," << endl;
    
    file << x << "," << y << "," << z << "," << endl;
    file << x << "," << -y << "," << -z << "," << endl;
    file << x << "," << y << "," << -z << "," << endl;

    //face 1
    file << x << "," << y << "," << z << "," << endl;
    file << -x << "," << y << "," << z << "," << endl;
    file << x << "," << -y << "," << z << "," << endl;

	file << x << "," << -y << "," << z << "," << endl;
    file << -x << "," << y << "," << z << "," << endl;
    file << -x << "," << -y << "," << z << "," << endl;

    //face 2
    file << -x << "," << -y << "," << z << "," << endl;
    file << -x << "," << y << "," << z << "," << endl;
    file << -x << "," << -y << "," << -z << "," << endl;

    file << -x << "," << -y << "," << -z << "," << endl;
    file << -x << "," << y << "," << z << "," << endl;
    file << -x << "," << y << "," << -z << "," << endl;

    //face 3
    file << -x << "," << -y << "," << -z << "," << endl;
    file << -x << "," << y << "," << -z << "," << endl;
    file << x << "," << y << "," << -z << "," << endl;

    file << -x << "," << -y << "," << -z << "," << endl;
    file << x << "," << y << "," << -z << "," << endl;
    file << x << "," << -y << "," << -z << "," << endl;



    file.close();
}


void esfera(float radius, int slices, int stacks, string fileName){
	ofstream file;
	file.open(fileName);
	int nVertices = 2*slices*stacks*3;
	float alpha = (2*M_PI)/slices;
	float beta = (M_PI)/stacks;

	file << nVertices << endl;

	/*
	for(int i = 0; i < slices; i++) {	

		float xDownLeft = radius * cos(M_PI/2 - beta) * sin(i * alpha);
		float yDownLeft = radius * sin(M_PI/2 - beta);
		float zDownLeft = radius * cos(M_PI/2 - beta) * cos(i * alpha);

		float xDownRight = radius * cos(M_PI/2 - beta) * sin((i + 1) * alpha);
		float yDownRight = yDownLeft;
		float zDownRight = radius * cos(M_PI/2 - beta) * cos((i + 1) * alpha);


		file << 0 << "," << radius << "," << 0 << "," << endl;
    	file << xDownLeft << "," << yDownLeft << "," << zDownLeft << "," << endl;
    	file << xDownRight << "," << yDownRight << "," << zDownRight << "," << endl;

	}*/
	//cada iteração desenha uma camada horizontal (do topo para baixo)
	for(int j = 0; j < stacks; j++) {
		//cada iteração desenha uma fatia da camada horizontal
		for(int i = 0; i < slices; i++) {
			//Vértice no canto superior esquerdo
			float xUpLeft = radius * cos(M_PI/2 - beta * j) * sin(i * alpha);
			float yUpLeft = radius * sin(M_PI/2 - beta * j);
			float zUpLeft = radius * cos(M_PI/2 - beta * j) * cos(i * alpha);
			//Vértice no canto superior direito
			float xUpRight = radius * cos(M_PI/2 - beta * j) * sin((i + 1) * alpha);
			float yUpRight = yUpLeft;
			float zUpRight = radius * cos(M_PI/2 - beta * j) * cos((i + 1) * alpha);
			//Vértice no canto inferior esquerdo
			float xDownLeft = radius * cos(M_PI/2 - beta * (j+1)) * sin(i * alpha);
			float yDownLeft = radius * sin(M_PI/2 - beta * (j+1));
			float zDownLeft = radius * cos(M_PI/2 - beta * (j+1)) * cos(i * alpha);
			//Vértice no canto inferior direito
			float xDownRight = radius * cos(M_PI/2 - beta * (j+1)) * sin((i + 1) * alpha);
			float yDownRight = yDownLeft;
			float zDownRight = radius * cos(M_PI/2 - beta * (j+1)) * cos((i + 1) * alpha);

			//triângulo com 2 vértices em baixo e um em cima
	        file << xDownLeft << "," << yDownLeft << "," << zDownLeft << "," << endl;
    		file << xDownRight << "," << yDownRight << "," << zDownRight << "," << endl;
    		file << xUpLeft << "," << yUpLeft << "," << zUpLeft << "," << endl;

    		//triângulo com 2 vértices em cima e um em baixo
    		file << xUpRight << "," << yUpRight << "," << zUpRight << "," << endl;
    		file << xUpLeft << "," << yUpLeft << "," << zUpLeft << "," << endl;
    		file << xDownRight << "," << yDownRight << "," << zDownRight << "," << endl;
		}
	}
    file.close();
}


void cone(float radius, float height, int slices, int stacks, string fileName){
	ofstream file;
	file.open(fileName);

	int nVertices = 2*slices*stacks*3;
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

	file << nVertices << endl;

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
		string dir1 = "../Models/";
		dir1.insert(10,argv[argc-1]);
		cout << dir1 << endl;

		if(strcmp(argv[1], "Plano") == 0 && argc == 4){
		    plano(atoi(argv[2]), dir1);
		}

		else if(strcmp(argv[1], "Caixa") == 0 && argc == 6){
		    caixa(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), dir1);
		}

		else if(strcmp(argv[1], "Esfera") == 0 && argc == 6){
		    esfera(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), dir1);
		}

		else if(strcmp(argv[1], "Cone") == 0 && argc == 7){
		    cone(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), dir1);
		}

		else{
			cout << "Introduza os parâmetros corretamente" << endl;
			cout << "Faça ./generator -h para ajuda." << endl;
		}
		return 1;
	}
}