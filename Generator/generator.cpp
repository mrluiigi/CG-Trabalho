#include "generator.h"
#include <string>
#include <math.h>

void plano(int lado, string ficheiro){
    ofstream file;
    file.open(ficheiro);

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

void caixa(float x, float y, float z, string ficheiro){
	ofstream file;
	file.open(ficheiro);

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


void esfera(float radius, int slices, int stacks, string ficheiro){
	ofstream file;
	file.open(ficheiro);
	int nVertices = 2*slices*stacks*3;
	float alpha = (2*M_PI)/slices;
	float beta = (M_PI)/stacks;
	float x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4, x5, y5, z5, x6, y6, z6;

	file << nVertices << endl;

	
	float xDownLeft = radius *sin((0) * alpha);
	float yDownLeft = 0;
	float zDownLeft = radius * cos((0) * alpha);

	
	for(int i = 0; i < slices; i++) {		
		float xDownRight = radius *sin((i+1) * alpha);
		float yDownRight = 0;
		float zDownRight = radius * cos((i+1) * alpha);

		float xTemp = xDownRight;
		float yTemp = yDownRight;
		float zTemp = zDownRight;

	    for(int j = 0; j < stacks; j++) {
	        
	        float xUpLeft = radius * cos(beta * (j+1)) * sin(i * alpha);
	        float yUpLeft = radius * sin(beta * (j+1));
	        float zUpLeft = radius * cos(beta * (j+1)) * cos(i * alpha); 

	        float xUpRight = radius * cos(beta * (j+1)) * sin((i + 1) * alpha);
	        float yUpRight = yUpLeft;
	        float zUpRight = radius * cos(beta * (j+1)) * cos((i + 1) * alpha); 

	        //baixo para cima
	        file << xDownLeft << "," << yDownLeft << "," << zDownLeft << "," << endl;
    		file << xDownRight << "," << yDownRight << "," << zDownRight << "," << endl;
    		file << xUpLeft << "," << yUpLeft << "," << zUpLeft << "," << endl;

    		//cima para baixo
    		file << xUpRight << "," << yUpRight << "," << zUpRight << "," << endl;
    		file << xUpLeft << "," << yUpLeft << "," << zUpLeft << "," << endl;
    		file << xDownRight << "," << yDownRight << "," << zDownRight << "," << endl;


    		//metade de baixo da esfera
    		file << xDownLeft << "," << -yDownLeft << "," << zDownLeft << "," << endl;
            file << xDownRight << "," << -yDownRight << "," << zDownRight << "," << endl;
            file << xUpLeft << "," << -yUpLeft << "," << zUpLeft << "," << endl;

            file << xUpRight << "," << -yUpRight << "," << zUpRight << "," << endl;
            file << xUpLeft << "," << -yUpLeft << "," << zUpLeft << "," << endl;
            file << xDownRight << "," << -yDownRight << "," << zDownRight << "," << endl;

            xDownLeft = xUpLeft;
            yDownLeft = yUpLeft;
            zDownLeft = zUpLeft;

            xDownRight = xUpRight;
            yDownRight = yUpRight;
            zDownRight = zUpRight;
	    }

	    xDownLeft = xTemp;
        yDownLeft = yTemp;
        zDownLeft = zTemp;

	}

    file.close();
}



void cone(float radius, float height, int slices, int stacks, string ficheiro){
	ofstream file;
	file.open(ficheiro);

	int nVertices = 2*slices*stacks*3;
	float alpha = (2*M_PI)/slices;

	float ladeira = sqrt(height*height+radius*radius);
	float divisaoPequena = (ladeira/stacks);
	float beta = atan(height/radius); //angulo entre a base do cone e a superficie lateral

	float y = 0;
	float bx1, by1, bz1, bx2, by2, bz2, bx3, by3, bz3;
	float x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4, x5, y5, z5, x6, y6, z6;

	float qtdSubRadius = cos(beta) * divisaoPequena;
	float qtdAddY = sin(beta) * divisaoPequena;

	file << nVertices << endl;

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

	for(int j = 0; j < stacks; j++) {

		float novoRaio = radius - qtdSubRadius;
		float novoY = y + qtdAddY;

		for(int i = 0; i < slices; i++) {

			//em baixo esquerda
			x1 = radius*sin(alpha*i);
			y1 = y;
			z1 = radius*cos(alpha*i);

			//em baixo direita
			x2 = radius*sin(alpha*(i+1));
			y2 = y;
			z2 = radius*cos(alpha*(i+1));

			//em cima esquerda
			x3 = novoRaio*sin(alpha*i);
			y3 = novoY;
			z3 = novoRaio*cos(alpha*i);



			//em cima direita
			x4 = novoRaio*sin(alpha*(i+1));
			y4 = novoY;
			z4 = novoRaio*cos(alpha*(i+1));

			//em cima esquerda
			x5 = novoRaio*sin(alpha*i);
			y5 = novoY;
			z5 = novoRaio*cos(alpha*i);

			//em baixo direita
			x6 = radius*sin(alpha*(i+1));
			y6 = y;
			z6 = radius*cos(alpha*(i+1));


			file << x1 << "," << y1 << "," << z1 << "," << endl;
    		file << x2 << "," << y2 << "," << z2 << "," << endl;
    		file << x3 << "," << y3 << "," << z3 << "," << endl;

    		file << x4 << "," << y4 << "," << z4 << "," << endl;
    		file << x5 << "," << y5 << "," << z5 << "," << endl;
    		file << x6 << "," << y6 << "," << z6 << "," << endl;
		}

		y = novoY;
		radius = novoRaio;
	}

    file.close();
}
























int main(int argc, char **argv){
	


	if(argc > 1){
		string dir1 = "../Models/";
		dir1.insert(10,argv[argc-1]);
		cout << dir1;

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
			printf("Introduza os parâmetros corretamente\n");
		}

	}
	else{
		printf("Parâmetros insuficientes\n");
	}
}