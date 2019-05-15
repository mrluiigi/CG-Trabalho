using namespace std;

#include <string>
#include <vector>

class ModelInfo{
    public:
        //nome do ficheiro a partir do qual o modelo foi carregado  
        string name;
        int numberOfIndices;
        int* indicesBuffer;
        int numberOfVertices;
        float* verticesBuffer;
        int numberOfNormals;
        float* normalsBuffer;
        int numberOfTextures;
        float* texturesBuffer;

};

class Texture{
    public:
        string name;
        int textureId;
};

class Colour{
    public:
        //Red Green Blue
        float colour[4];
};

class Model{
    public:    
        bool hasTexture;
        Texture* texture;
        ModelInfo* modelInfo;
        Colour* diffuse;
        Colour* specular;
        Colour* emissive;
        Colour* ambient;
        float shininess;

};

class Models{
    public:
        vector<ModelInfo*> vec;

	    int contains(string name, string texture);
        //Retorna um identificador para o modelo adicionado, se já existir um modelo com o mesmo nome retorna o identificador desse
	    ModelInfo* addModel(string filename);

	    //preenche o vetor vertices de cada modelo com os dados presentes nos respetivos ficheiros
	    void loadModelsInfo();			
        // Devolve o model dado o seu identificador 
	    ModelInfo* getModel(string filename);
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
    vector<Model> models;
    vector<Group> subGroups;

};

class Light{
public:
    //Vetor com os Ids(posição no vetor do objeto Models) dos modelos 
    string type;

    float posX;
    float posY;
    float posZ;

    float dirX;
    float dirY;
    float dirZ;
    
    float cutoffAngle;
    float exponent;

    Colour* diffuse;
    Colour* specular;
    Colour* emissive;
    Colour* ambient;

};

/**Recebe o nome do ficheiro XML onde se encontra a configuração.
  Recebe uma referência para um objecto da classe Models para o preencher com a informação dos ficheiros .3d
  Recebe uma referência para um vetor de objetos Group para o preencher 
  com a informação dos grupos presente no ficheiro configuração	
*/
void loadConfig(char* fileXML, vector<Group>& groups, vector<Light>& lights, vector<Texture*>& textures);