#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
using namespace std; //com isto posso escrever string e o compilador vai perceber

void plano(int lado, string ficheiro);

void cubo(float comp, float larg, float altura, int divisions);

void sphere(float radius, int slices, int stacks);

void cone(float radius, float height, int slices, int stacks);