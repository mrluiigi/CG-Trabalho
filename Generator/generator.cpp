#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
using namespace std; //com isto posso escrever string e o compilador vai perceber

void plano(int lado, string ficheiro){

    ofstream file;
    file.open(ficheiro);
    float x, y, z;
    y = 0;
    x = lado/2;
    z = lado/2;


	file << x << "," << y << "," << z << endl; //isto é um vértice
	file << x << "," << y << "," << -z << endl; 
	file << -x << "," << y << "," << -z << endl; 

	file << -x << "," << y << "," << -z << endl; 
	file << -x << "," << y << "," << z << endl; 
	file << x << "," << y << "," << z << endl; 


    file.close();
}

int main(int argc, char **argv){
	if(argc > 1){

		if(strcmp(argv[1], "Plano") == 0){
		    plano(atoi(argv[2]), argv[3]);
		}
		else{
			printf("Erro\n");
		}

	}
	else{
		printf("Parâmetros insuficientes\n");
	}
}