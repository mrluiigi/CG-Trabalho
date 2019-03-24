#include "loadConfig.h"

#include <fstream>
#include <iostream>
#include "tinyxml2.h"

using namespace tinyxml2;



//obtem um vértice a partir de uma linha do ficheiro .3d
Vertice toVertice(string s){
    Vertice v;

    float array[3];
    int i = 0;

    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) { //encontrar posição da vírgula
        token = s.substr(0, pos);   //obter substring entre virgulas
        array[i] = stof(token);     //transformar substring em float
        i++;
        s.erase(0, pos + delimiter.length());
    }
    v.x = array[0];
    v.y = array[1];
    v.z = array[2];
    return v;
}


bool Models::contains(string name) {
    for (int i = 0; i < vec.size(); i++){
        if(name.compare(vec[i].name) == 0) {
            return true;
        }
    }
    return false;
}
//Não adiciona repetidos
void Models::addModel(string name) {
    if(!contains(name)) {
        Model m;
        m.name = name;
        vec.push_back(m);
    }
}

//preenche o vetor vertices de cada modelo com os dados presentes nos respetivos ficheiros
void Models::loadModels() {
    for (int i = 0; i < vec.size(); i++){   
        Model &m = vec[i];
        //Abre o ficheiro .3d
        ifstream file(m.name);
        string s;
        //Para cada linha processa um vértice
        while(getline(file, s)){
            Vertice v = toVertice(s);
            m.vertices.push_back(v);
        }
        file.close();
    }
}

vector<Vertice>& Models::getModel(string name) {
    for (int i = 0; i < vec.size(); i++){
        if(name.compare(vec[i].name) == 0) {
            return vec[i].vertices;
        }
    }
}


Group parseGroup(tinyxml2::XMLElement *group, Models& allModels){
    Group g;
    for(tinyxml2::XMLElement *child = group->FirstChildElement(); child != NULL; child = child->NextSiblingElement()){
        string childName = string(child->Name());

        if( childName.compare("translate") == 0 || childName.compare("scale") == 0) {
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
            gt.angle = atof(child->Attribute("angle"));
            gt.type = 1;
            g.transforms.push_back(gt);
        }  

        else if( string(child->Name()).compare("models") == 0) {
                for(tinyxml2::XMLElement *modelXML = child->FirstChildElement(); modelXML != NULL; modelXML = modelXML->NextSiblingElement()){
                    string name = modelXML->Attribute("file");
                    //Guarda apenas o nome do ficheiro 3d
                    //O ficheiro será processado quando for invocada a função loadModels()
                    allModels.addModel(name);
                    g.models.push_back(name);
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