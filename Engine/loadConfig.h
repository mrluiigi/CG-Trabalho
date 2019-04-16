using namespace std;

#include <string>
#include <vector>


class Vertice{
    public:
        float x;
        float y;
        float z;
};

class Model{
    public:
    	//nome do ficheiro a partir do qual o modelo foi carregado	
        string name;
		int numberOfIndices;
		int* indicesBuffer;
		int numberOfVertices;
		vector<Vertice> vertices;
        float* verticesBuffer;

};

class Models{
    public:
        vector<Model> vec;

	    bool contains(string name);

	    void addModel(string name);

	    //preenche o vetor vertices de cada modelo com os dados presentes nos respetivos ficheiros
	    void loadModels();			

	    Model getModel(string name);
};




class TimedTranslate{
public:
    float time;
    int controlPointsNumber;
    float** controlPoints;
};

class GeometricTransforms {
public:
    //0 -> translate, 1 -> rotate, 2 -> scale, 3 -> timed translate
    int type;
    TimedTranslate * tt;
    float x;
    float y;
    float z;
    //Utilizado caso a transformação seja uma rotação
    float angle;
};

class Group{
public:
    vector<GeometricTransforms> transforms;
    vector<string> models;
    vector<Group> subGroups;
    vector<TimedTranslate> timedTranslates;

};

/**Recebe o nome do ficheiro XML onde se encontra a configuração.
  Recebe uma referência para um objecto da classe Models para o preencher com a informação dos ficheiros .3d
  Recebe uma referência para um vetor de objetos Group para o preencher 
  com a informação dos grupos presente no ficheiro configuração	
*/
void loadConfig(char* fileXML, Models& allModels, vector<Group>& groups);