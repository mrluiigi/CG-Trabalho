#include "loadConfig.h"

#include <fstream>
#include <iostream>
#include "tinyxml2.h"

using namespace tinyxml2;


/** Obtém um vértice a partir de uma linha do ficheiro .3d */
void toVertice(string s, float* res){

    int i = 0;

    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) { //encontrar posição da vírgula
        token = s.substr(0, pos);   //obter substring entre virgulas
        res[i] = stof(token);     //transformar substring em float
        i++;
        s.erase(0, pos + delimiter.length());
    }
}

void toIndice(string s, int* array){
    int i = 0;

    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) { //encontrar posição da vírgula
        token = s.substr(0, pos);   //obter substring entre virgulas
        array[i] = stoi(token);     //transformar substring em float
        i++;
        s.erase(0, pos + delimiter.length());
    }
}

//Se contem retorna o índice do modelo, caso contrário retorna -1
int Models::contains(string name) {
    for (int i = 0; i < vec.size(); i++){
        if(name.compare(vec[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

/** Adiciona o model dado o nome do modelo */
//Retorna um identificador para o modelo adicionado, se já existir um modelo com o mesmo nome retorna o identificador desse
int Models::addModel(string name) {
    int id = contains(name);
    if(id == -1) {
        Model m;
        m.name = name;
        vec.push_back(m);
        return vec.size()-1;
    }
    else return id;
}

/** Preenche o vetor vertices de cada modelo com os dados presentes nos respetivos ficheiros */
void Models::loadModels() {
    for (int i = 0; i < vec.size(); i++){
        Model &m = vec[i];

        //Abre o ficheiro .3d
        ifstream file(m.name);
        string s;

        //Lê o número de índices presente na 1º linha
        getline(file,s);
        m.numberOfIndices = stoi(s);
        m.indicesBuffer = (int *)malloc(sizeof(int) * m.numberOfIndices);

        //Lê os índices
        getline(file,s);
        toIndice(s, m.indicesBuffer);

        //Lê o número de vértices presenta na 3º linha
        getline(file,s);
        m.numberOfVertices = stoi(s);
        m.verticesBuffer = (float *)malloc(sizeof(float) * m.numberOfVertices * 3);

        int offset = 0;

        //Para cada linha processa um vértice
        while(getline(file, s)){
            toVertice(s, m.verticesBuffer + offset);
            //Cada vértice é constituido por 3 floats
            offset += 3;
        }

        file.close();
    }
}

// Devolve o model dado o seu identificador 
Model Models::getModel(int id) {
    return vec[id];
}

void parseTimedTranslate(tinyxml2::XMLElement * element, TimedTranslate* tt) {
    tt->time = atoi(element->Attribute("time"));
    int c = 0;
    //Vetor temporário para armazenar os pontos de controlo
    vector<float> points;
    for(tinyxml2::XMLElement *child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()){
        points.push_back(atof(child->Attribute("X")));
        points.push_back(atof(child->Attribute("Y")));
        points.push_back(atof(child->Attribute("Z")));
        c++;
    }
    //Alocar 

    tt->controlPoints = new float*[c];    
    for (int i = 0; i < c; i++) {
        tt->controlPoints[i] = new float[3];
    }


    printf("..\n");
    for (int i = 0; i < c; i++) {
        tt->controlPoints[i][0] = points[i*3+0];
        tt->controlPoints[i][1] = points[i*3+1];
        tt->controlPoints[i][2] = points[i*3+2];
    }
    printf("deu\n");

    tt->controlPointsNumber = c;
}

Group parseGroup(tinyxml2::XMLElement *group, Models& allModels){
    Group g;
    for(tinyxml2::XMLElement *child = group->FirstChildElement(); child != NULL; child = child->NextSiblingElement()){
        string childName = string(child->Name());
        //Se for uma translação dependente do tempo
        if( childName.compare("translate") == 0 &&  child->Attribute("time") != NULL) {
            GeometricTransforms gt;
            gt.type = 3;
            gt.tt = new TimedTranslate;
            parseTimedTranslate(child,gt.tt);
            g.transforms.push_back(gt);
        }


        else if( childName.compare("translate") == 0 || childName.compare("scale") == 0) {
            GeometricTransforms gt;
            gt.x = atof(child->Attribute("X"));
            gt.y = atof(child->Attribute("Y"));
            gt.z = atof(child->Attribute("Z"));
            if ( childName.compare("translate") == 0) {
                gt.type = 0;
            }
            else if ( childName.compare("scale") == 0) {
                gt.type = 2;
            }
            g.transforms.push_back(gt);
        }

        else if( childName.compare("rotate") == 0) {
            GeometricTransforms gt;
            gt.x = atof(child->Attribute("axisX"));
            gt.y = atof(child->Attribute("axisY"));
            gt.z = atof(child->Attribute("axisZ"));
            //Se não for uma rotação dependente do tempo
            if(child->Attribute("angle") != NULL){
                gt.angle = atof(child->Attribute("angle"));
            	gt.type = 1;
            }
            //Se for uma rotação dependente do tempo
            else{
            	gt.rotateTime = atoi(child->Attribute("time"));
            	gt.type = 4;
            }
            g.transforms.push_back(gt);
        }  

        else if( string(child->Name()).compare("models") == 0) {
                for(tinyxml2::XMLElement *modelXML = child->FirstChildElement(); modelXML != NULL; modelXML = modelXML->NextSiblingElement()){
                    string name = modelXML->Attribute("file");
                    //Guarda apenas o nome do ficheiro 3d
                    //O ficheiro será processado quando for invocada a função loadModels()
                    int id = allModels.addModel(name);
                    g.models.push_back(id);
                }
        }
        //caso o grupo contenha subgrupos
        else if( childName.compare("group") == 0) {
            g.subGroups.push_back(parseGroup(child, allModels));
        }
    }
    return g;
}


/** Parse ao ficheiro XML */
void parseXML(char* file, Models& allModels, vector<Group>& groups){
    tinyxml2::XMLDocument doc;

    //Se conseguir carregar o ficheiro
    if(!(doc.LoadFile(file))) {
        tinyxml2::XMLElement* root = doc.FirstChildElement();
        for(tinyxml2::XMLElement *child = root->FirstChildElement(); child != NULL; child = child->NextSiblingElement()){
            if( string(child->Name()).compare("group") == 0) {
                Group g = parseGroup(child, allModels);
                groups.push_back(g);
            }
        }
    }
}

/** Processa os vértices lidos do ficheiro e coloca o modelo obtido no vector de modelos global*/
void loadConfig(char* fileXML, Models& allModels, vector<Group>& groups){
    parseXML(fileXML, allModels,groups);
    allModels.loadModels();
}