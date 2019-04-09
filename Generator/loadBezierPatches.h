using namespace std; 
#include <string>

class BezierPatches {
	public :
		int numberOfPatches;
		//patches[numberOfPatches][16]
		int** patches;
		int numberOfPoints;
		//points[numberOfPoints][3]
		float** points;

		//BezierPatches(int numberOfPatches, int**patches, int numberOfPoints, float** points) {


};

void loadBezierPatches(string filename);