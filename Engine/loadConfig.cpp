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

/** Obtém uma normal a partir de uma linha do ficheiro .3d */
void toNormal(string s, float* res){

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
/*
//Se contem retorna o índice do modelo, caso contrário retorna -1
int Models::contains(string name, string texture) {
    for (int i = 0; i < vec.size(); i++){
        if(name.compare(vec[i].name) == 0 && texture.compare(vec[i].texture) == 0) {
            return i;
        }
    }
    return -1;
}*/

// Devolve o model dado o seu identificador 
ModelInfo* Models::getModel(string filename) {
    for (int i = 0; i < vec.size(); i++){
        ModelInfo* mip = vec[i];
        if(filename.compare(mip->name) == 0){
            return mip;
        }
    }
    return NULL;
}

/** Adiciona o model dado o nome do modelo */
//Retorna um identificador para o modelo adicionado, se já existir um modelo com o mesmo nome retorna o identificador desse
ModelInfo* Models::addModel(string filename) {
    ModelInfo* mip = getModel(filename);
    if(mip == NULL) {
        mip = new ModelInfo;
        mip->name = filename;
        vec.push_back(mip);
        return mip;
    }
    else return mip;
}

/** Preenche o vetor vertices de cada modelo com os dados presentes nos respetivos ficheiros */
void Models::loadModelsInfo() {
    for (int i = 0; i < vec.size(); i++){
        ModelInfo* m = vec[i];

        //Abre o ficheiro .3d
        ifstream file(m->name);
        string s;

        //Lê o número de índices presente na 1º linha
        getline(file,s);
        m->numberOfIndices = stoi(s);
        m->indicesBuffer = (int *)malloc(sizeof(int) * m->numberOfIndices);

        //Lê os índices
        getline(file,s);
        toIndice(s, m->indicesBuffer);

        //Lê o número de vértices presenta na 3º linha
        getline(file,s);
        m->numberOfVertices = stoi(s);
        m->verticesBuffer = (float *)malloc(sizeof(float) * m->numberOfVertices * 3);

        int offset = 0;

        //Para cada linha processa um vértice
        for(int i = 0; i < m->numberOfVertices; i++){
            getline(file, s);
            toVertice(s, m->verticesBuffer + offset);
            //Cada vértice é constituido por 3 floats
            offset += 3;
        }

        //Lê o número de normais
        m->numberOfNormals = m->numberOfVertices;
        m->normalsBuffer = (float *)malloc(sizeof(float) * m->numberOfNormals * 3);

        offset = 0;

        //Para cada linha processa uma normal
        for(int i = 0; i < m->numberOfNormals; i++){
            getline(file, s);
            toNormal(s, m->normalsBuffer + offset);
            //Cada normal é constituida por 3 floats
            offset += 3;
        }


        m->numberOfTextures = m->numberOfVertices;
        m->texturesBuffer = (float *)malloc(sizeof(float) * m->numberOfTextures * 2);

        offset = 0;
        
        for(int i = 0; i < m->numberOfTextures; i++){
            getline(file, s);
            toNormal(s, m->texturesBuffer + offset);
            //Cada textura é constituida por 2 floats
            offset += 2;
        }




        file.close();
    }
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
    //Alocar matriz 2D dos pontos de controlo
    tt->controlPoints = new float*[c];    
    for (int i = 0; i < c; i++) {
        tt->controlPoints[i] = new float[3];
    }
    //Colocar pontos de controlo na matriz
    for (int i = 0; i < c; i++) {
        tt->controlPoints[i][0] = points[i*3+0];
        tt->controlPoints[i][1] = points[i*3+1];
        tt->controlPoints[i][2] = points[i*3+2];
    }
    tt->controlPointsNumber = c;
}
Colour* parseDiffuse(tinyxml2::XMLElement *xml){
    Colour* diffuse = NULL;
    if(xml->Attribute("diffR") != NULL){
        diffuse = new Colour;
        diffuse->colour[0] = atof(xml->Attribute("diffR"));
        diffuse->colour[1] = atof(xml->Attribute("diffG"));
        diffuse->colour[2] = atof(xml->Attribute("diffB"));
        diffuse->colour[3] = atof(xml->Attribute("diffA"));
    }
    return diffuse;
}

Colour* parseSpecular(tinyxml2::XMLElement *xml){
    Colour* specular = NULL;
    if(xml->Attribute("specR") != NULL){
        specular = new Colour;
        specular->colour[0] = atof(xml->Attribute("specR"));
        specular->colour[1] = atof(xml->Attribute("specG"));
        specular->colour[2] = atof(xml->Attribute("specB"));
        specular->colour[3] = atof(xml->Attribute("specA"));
    }
    return specular;
}
Colour* parseEmissive(tinyxml2::XMLElement *xml){
    Colour* emissive = NULL;
    if(xml->Attribute("emiR") != NULL){
        emissive = new Colour;
        emissive->colour[0] = atof(xml->Attribute("emiR"));
        emissive->colour[1] = atof(xml->Attribute("emiG"));
        emissive->colour[2] = atof(xml->Attribute("emiB"));
        emissive->colour[3] = atof(xml->Attribute("emiA"));
    }
    return emissive;
}

Colour* parseAmbient(tinyxml2::XMLElement *xml){
    Colour* ambient = NULL;
    if(xml->Attribute("ambR") != NULL){
        ambient = new Colour;
        ambient->colour[0] = atof(xml->Attribute("ambR"));
        ambient->colour[1] = atof(xml->Attribute("ambG"));
        ambient->colour[2] = atof(xml->Attribute("ambB"));
        ambient->colour[3] = atof(xml->Attribute("ambA"));
    }
    return ambient;
}

Group parseGroup(tinyxml2::XMLElement *group, Models& allModels, vector<Texture*>& textures){
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
                    Model m;
                    string filename = modelXML->Attribute("file");
                    Texture* txt = NULL;
                    if(modelXML->Attribute("texture") != NULL){
                        string textureName = modelXML->Attribute("texture");
                        for (int i = 0; i < textures.size(); i++){
                            if(textureName.compare(textures[i]->name) == 0){
                                txt = textures[i];
                            }
                        }
                        if(txt == NULL){
                            txt = new Texture;
                            txt->name = textureName;
                            textures.push_back(txt);                         
                        }
                        m.texture = txt;
                        m.hasTexture = true;
                    }
                    else{
                        m.hasTexture = false;
                    }
                    m.diffuse = parseDiffuse(modelXML);
                    m.specular = parseSpecular(modelXML);
                    m.emissive = parseEmissive(modelXML);
                    m.ambient = parseAmbient(modelXML);
                    //Guarda apenas o nome do ficheiro 3d
                    //O ficheiro será processado quando for invocada a função loadModels()
                    m.modelInfo = allModels.addModel(filename);
                    g.models.push_back(m);
                }
        }
        //caso o grupo contenha subgrupos
        else if( childName.compare("group") == 0) {
            g.subGroups.push_back(parseGroup(child, allModels, textures));
        }
    }
    return g;
}



void parseLights(tinyxml2::XMLElement *light, vector<Light>& lights){
    for(tinyxml2::XMLElement *child = light->FirstChildElement(); child != NULL; child = child->NextSiblingElement()){

        Light l;
        string type = string(child->Attribute("type"));


        if( type.compare("POINT") == 0 ) {
            l.type = "POINT";

            l.posX = atof(child->Attribute("posX"));
            l.posY = atof(child->Attribute("posY"));
            l.posZ = atof(child->Attribute("posZ"));
        }


        else if( type.compare("DIRECTIONAL") == 0) {
            l.type = "DIRECTIONAL";

            l.dirX = atof(child->Attribute("dirX"));
            l.dirY = atof(child->Attribute("dirY"));
            l.dirZ = atof(child->Attribute("dirZ"));
        }


        else if( type.compare("SPOTLIGHT") == 0 ) {
            l.type = "SPOTLIGHT";

            l.posX = atof(child->Attribute("posX"));
            l.posY = atof(child->Attribute("posY"));
            l.posZ = atof(child->Attribute("posZ"));        

            l.dirX = atof(child->Attribute("dirX"));
            l.dirY = atof(child->Attribute("dirY"));
            l.dirZ = atof(child->Attribute("dirZ"));

            l.cutoffAngle = atof(child->Attribute("cutoffAngle"));
            l.exponent = atof(child->Attribute("exponent"));
        }

        l.diffuse = parseDiffuse(child);
        l.specular = parseSpecular(child);
        l.emissive = parseEmissive(child);
        l.ambient = parseAmbient(child);

        lights.push_back(l);
    }
}







/** Parse ao ficheiro XML */
void parseXML(char* file, Models& allModels, vector<Group>& groups, vector<Light>& lights, vector<Texture*>& textures){
    tinyxml2::XMLDocument doc;

    //Se conseguir carregar o ficheiro
    if(!(doc.LoadFile(file))) {
        tinyxml2::XMLElement* root = doc.FirstChildElement();
        for(tinyxml2::XMLElement *child = root->FirstChildElement(); child != NULL; child = child->NextSiblingElement()){
            if( string(child->Name()).compare("group") == 0) {
                Group g = parseGroup(child, allModels, textures);
                groups.push_back(g);
            }
            else if( string(child->Name()).compare("lights") == 0) {
                parseLights(child, lights);
            }
        }
    }
}

/** Processa os vértices lidos do ficheiro e coloca o modelo obtido no vector de modelos global*/
void loadConfig(char* fileXML, vector<Group>& groups, vector<Light>& lights, vector<Texture*>& textures){
    Models allModels;
    parseXML(fileXML, allModels,groups, lights,textures);
    allModels.loadModelsInfo();
}