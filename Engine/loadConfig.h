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
        string name;
        vector<Vertice> vertices;
};

class Models{
    public:
        vector<Model> vec;

	    bool contains(string name);

	    void addModel(string name);

	    void loadModels();

	    vector<Vertice>& getModel(string name);
};

//0 -> translate
//1 -> rotate
//2 -> scale

class GeometricTransforms {
public:
    int type;
    float x;
    float y;
    float z;
    float angle;
};


class Group{
public:
    vector<GeometricTransforms> transforms;
    vector<string> models;
    vector<Group> subGroups;


};

void loadConfig(char* fileXML, Models& allModels, vector<Group>& groups);