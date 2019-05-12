using namespace std;

#include <string>
#include <vector>



class Model{
    public:
    	//nome do ficheiro a partir do qual o modelo foi carregado	
        string name;
        string texture;
        int textureId;
        bool hasTexture;
		int numberOfIndices;
		int* indicesBuffer;
		int numberOfVertices;
        float* verticesBuffer;
        int numberOfNormals;
        float* normalsBuffer;
        int numberOfTextures;
        float* texturesBuffer;

};

class Models{
    public:
        vector<Model> vec;

	    int contains(string name, string texture);
        //Retorna um identificador para o modelo adicionado, se já existir um modelo com o mesmo nome retorna o identificador desse
	    int addModel(Model m);

	    //preenche o vetor vertices de cada modelo com os dados presentes nos respetivos ficheiros
	    void loadModels();			
        // Devolve o model dado o seu identificador 
	    Model getModel(int id);
};




class TimedTranslate{
public:
    int time;
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
    //Utilizado caso a transformação seja uma rotação temporal
    int rotateTime;
};

class Group{
public:
    vector<GeometricTransforms> transforms;
    //Vetor com os Ids(posição no vetor do objeto Models) dos modelos 
    vector<int> models;
    vector<Group> subGroups;

};

/**Recebe o nome do ficheiro XML onde se encontra a configuração.
  Recebe uma referência para um objecto da classe Models para o preencher com a informação dos ficheiros .3d
  Recebe uma referência para um vetor de objetos Group para o preencher 
  com a informação dos grupos presente no ficheiro configuração	
*/
void loadConfig(char* fileXML, Models& allModels, vector<Group>& groups);