#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
using namespace std; //com isto posso escrever string e o compilador vai perceber

void plano(int lado, string ficheiro);

void caixa(float x, float y, float z, string ficheiro);

void esfera(float radius, int slices, int stacks, string ficheiro);

void cone(float radius, float height, int slices, int stacks, string ficheiro);