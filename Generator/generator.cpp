#include "generator.h"
#include <string>

void plano(int lado, string ficheiro){
    ofstream file;
    file.open(ficheiro);

    float x, y, z;
    y = 0;
    x = lado/2;   //Verificar se está bem assim.
    z = lado/2;

    file << 6 << endl;

    //Escreve os vértices de um triangulo no ficheiro
	file << x << "," << y << "," << z << "," << endl; 
	file << x << "," << y << "," << -z << "," << endl; 
	file << -x << "," << y << "," << -z << "," << endl; 

	//Vértices do outro triângulo
	file << -x << "," << y << "," << -z << "," << endl; 
	file << -x << "," << y << "," << z << "," << endl; 
	file << x << "," << y << "," << z << "," << endl; 


    file.close();
}


int main(int argc, char **argv){
	


	if(argc > 1){
		string dir1 = "../Models/";
		dir1.insert(10,argv[3]);
		cout << dir1;

		if(strcmp(argv[1], "Plano") == 0 && argc == 4){
		    plano(atoi(argv[2]), dir1);
		}
		else{
			printf("Introduza os parâmetros corretamente\n");
		}

	}
	else{
		printf("Parâmetros insuficientes\n");
	}
}