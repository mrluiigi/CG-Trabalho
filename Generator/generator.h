#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
using namespace std; 

void plano(float lado, string fileName);

void caixa(float x, float y, float z, int divisions, string fileName);

void esfera(float radius, int slices, int stacks, string fileName);

void cone(float radius, float height, int slices, int stacks, string fileName);