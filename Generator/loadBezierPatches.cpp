#include "loadBezierPatches.h"
#include <fstream>
#include <iostream>

int* lineToPatch(string line) {
	int* patch = new int[16];
    int i = 0;

    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = line.find(delimiter)) != std::string::npos) { //encontrar posição da vírgula
        token = line.substr(0, pos);   //obter substring entre virgulas
        patch[i] = stoi(token);     //transformar substring em float
        i++;
        line.erase(0, pos + delimiter.length());
    }
    patch[i] = stoi(line);
    return patch;
}


float* lineToPoint(string line) {
	float* point = new float[3];
    int i = 0;

    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = line.find(delimiter)) != std::string::npos) { //encontrar posição da vírgula
        token = line.substr(0, pos);   //obter substring entre virgulas
        point[i] = stof(token);     //transformar substring em float
        i++;
        line.erase(0, pos + delimiter.length());
    }
    point[i] = stof(line);
    return point;
}

BezierPatches* loadBezierPatches(string filename) {
	ifstream file(filename);
	string line;
	//ler número de patches
	getline(file, line);
	int numberOfPatches = stoi(line);
	int** patches = new int*[numberOfPatches];
	//ler patches
	for (int i = 0; i < numberOfPatches; i++) {
		getline(file, line);
		patches[i] = lineToPatch(line);
	}
	//lê número de pontos de controlo
	getline(file, line);
	int numberOfPoints = stoi(line);
	float** points = new float*[numberOfPoints];
	//lê pontos de controlo
	for (int i = 0; i < numberOfPoints; i++) {
		getline(file, line);
		points[i] = lineToPoint(line);
	}

	BezierPatches* bezierPatches = new BezierPatches();
	bezierPatches->numberOfPatches = numberOfPatches;
	bezierPatches->patches = patches;
	bezierPatches->numberOfPoints = numberOfPoints;
	bezierPatches->points = points;



	return bezierPatches;

}