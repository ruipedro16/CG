#include "parser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "light.hpp"
#include "material.hpp"
#include "transform.hpp"

#define POINT 1
#define DIRECTIONAL 2
#define DIFFUSE 3
#define AMBIENT 4
#define SPECULAR 5

using namespace std;

int read_file(const char *path, vector<Vertex *> *vertices, vector<Vertex *> *normals,
              vector<float> *texture) {
    ifstream file(path);
    if (!file.is_open()) {
        cout << "Could not open file " << path << endl;
        return 1;
    }
    int index, j;
    string token, l, t;
    vector<float> tokens;
    index = 0;
    getline(file, l);
    // Vertices
    int numVertex = atoi(l.c_str());
    for (int i = 0; i < numVertex; i++) {
        getline(file, l);
        stringstream ss(l);
        j = 0;

        while (j < 3 && getline(ss, token, ' ')) {
            tokens.push_back(stof(token));
            j++;
        }

        vertices->push_back(new Vertex(tokens[index++], tokens[index++], tokens[index++]));
    }

    // Normals
    index = 0;
    getline(file, l);
    int numNormals = atoi(l.c_str());
    for (int i = 0; i < numNormals; i++) {
        getline(file, l);
        stringstream ss(l);
        j = 0;

        while (j < 3 && getline(ss, token, ' ')) {
            tokens.push_back(stof(token));
            j++;
        }

        normals->push_back(new Vertex(tokens[index++], tokens[index++], tokens[index++]));
    }

    // Textures
    getline(file, l);
    int numTexture = atoi(l.c_str());

    for (int i = 0; i < numTexture; i++) {
        getline(file, l);
        stringstream ss(l);
        j = 0;

        while (j < 2 && getline(ss, token, ' ')) {
            j++;
            texture->push_back(stof(token));
        }
    }
    file.close();
    return 0;
}

Scene *load_XML_file(const char *path) {
    Scene *scene = nullptr;
    XMLDocument xmlDoc;
    XMLNode *pNode;
    XMLElement *pElement;

    if (xmlDoc.LoadFile(path) == XML_SUCCESS) {
        Group *group = new Group();
        scene = new Scene(group);
        pNode = xmlDoc.FirstChild();

        if (pNode != nullptr) {
            pElement = pNode->FirstChildElement("lights");

            if (pElement) {
                pElement->FirstChildElement();
                parse_lights(scene, pElement);
            }
            pElement = pNode->FirstChildElement("group");
            parse_group(scene, group, pElement);
        }
    } else {
        cout << "Could not open file " << path << endl;
    }
    return scene;
}

void parse_scale(Group *group, XMLElement *element) {
    float x = 1.0f, y = 1.0f, z = 1.0f;
    std::string type = "scale";

    if (element->Attribute("X")) x = stof(element->Attribute("X"));
    if (element->Attribute("Y")) y = stof(element->Attribute("Y"));
    if (element->Attribute("Z")) z = stof(element->Attribute("Z"));

    group->addTransform(new Transform(type, 0.0f, x, y, z));
}

void parse_rotate(Group *group, XMLElement *element) {
    float angle = 0.0f, x = 0.0f, y = 0.0f, z = 0.0f;
    string type = "rotation";

    if (element->Attribute("time")) {
        float time = stof(element->Attribute("time"));
        angle = 360 / (time * 1000);
        type = "rotateTime";
    }

    if (element->Attribute("angle")) angle = stof(element->Attribute("angle"));

    if (element->Attribute("X")) x = stof(element->Attribute("X"));
    if (element->Attribute("Y")) y = stof(element->Attribute("Y"));
    if (element->Attribute("Z")) z = stof(element->Attribute("Z"));

    group->addTransform(new Transform(type, angle, x, y, z));
}

void parse_translate(Group *group, XMLElement *element) {
    float x = 0.0f, y = 0.0f, z = 0.0f;

    if (element->Attribute("time")) {
        bool d = false;
        vector<Vertex *> points;

        if (element->Attribute("derivative")) d = stoi(element->Attribute("derivative")) == 1;

        float time = stof(element->Attribute("time"));
        time = 1 / (time * 1000);
        element = element->FirstChildElement("point");

        while (element != nullptr) {
            if (element->Attribute("X")) x = stof(element->Attribute("X"));

            if (element->Attribute("Y")) y = stof(element->Attribute("Y"));

            if (element->Attribute("Z")) z = stof(element->Attribute("Z"));

            points.push_back(new Vertex(x, y, z));
            element = element->NextSiblingElement("point");
        }

        group->addTransform(new Transform("translateTime", time, points, d));
    } else {
        if (element->Attribute("X")) x = stof(element->Attribute("X"));
        if (element->Attribute("Y")) y = stof(element->Attribute("Y"));
        if (element->Attribute("Z")) z = stof(element->Attribute("Z"));

        group->addTransform(new Transform("translation", 0, x, y, z));
    }
}

void parse_models(Group *group, XMLElement *element) {
    element = element->FirstChildElement("model");
    if (element == nullptr) {
        cout << "Could not find model" << endl;
        return;
    }

    vector<Shape *> shapes;

    while (element != nullptr) {
        string file = element->Attribute("file");
        string file_path = "../models/" + file;

        if (!file.empty()) {
            vector<Vertex *> vertices;
            vector<Vertex *> normal;
            vector<float> texture;
            read_file(file_path.c_str(), &vertices, &normal, &texture);

            if (vertices.size()) {
                Shape *shape;

                if (element->Attribute("texture")) {
                    shape = new Shape(element->Attribute("texture"), vertices, normal, texture);
                } else {
                    shape = new Shape(vertices, normal, texture);
                }

                parse_material(shape, element);
                shapes.push_back(shape);
            }
        }

        element = element->NextSiblingElement("model");
    }

    if (shapes.size()) group->setShapes(shapes);
}

void parse_group(Scene *scene, Group *group, XMLElement *gElement) {
    XMLElement *element = gElement->FirstChildElement();
    while (element) {
        if (!strcmp(element->Name(), "translate")) {
            parse_translate(group, element);
        } else if (!strcmp(element->Name(), "scale")) {
            parse_scale(group, element);
        } else if (!strcmp(element->Name(), "rotate")) {
            parse_rotate(group, element);
        } else if (!strcmp(element->Name(), "models")) {
            parse_models(group, element);
        } else if (!strcmp(element->Name(), "group")) {
            Group *child = new Group();
            group->addGroup(child);
            parse_group(scene, child, element);
        }
        element = element->NextSiblingElement();
    }
}

void parse_lights(Scene *scene, XMLElement *pElement) {
    if ((pElement = pElement->FirstChildElement("light")) == nullptr) {
        cout << "Error: No lighting defined" << endl;
        return;
    }
    while (pElement != nullptr) {
        if (pElement->Attribute("type")) {
            int type = -1;
            float *info = (float *)calloc(16, sizeof(float));
            vector<int> attributes;

            if (!strcmp(pElement->Attribute("type"), "POINT")) {
                type = POINT;
                info[3] = 1.0f;
            } else if (!strcmp(pElement->Attribute("type"), "DIRECTIONAL")) {
                type = DIRECTIONAL;
                info[3] = 0.0f;
            }

            // Light Position
            if (pElement->Attribute("posX")) {
                info[0] = stof(pElement->Attribute("posX"));
            } else if (pElement->Attribute("posY")) {
                info[1] = stof(pElement->Attribute("posY"));
            } else if (pElement->Attribute("posZ")) {
                info[2] = stof(pElement->Attribute("posZ"));
            }

            if (pElement->Attribute("diffR") || pElement->Attribute("diffG") ||
                pElement->Attribute("diffB")) {
                attributes.push_back(DIFFUSE);

                if (pElement->Attribute("diffR")) info[4] = stof(pElement->Attribute("diffR"));
                if (pElement->Attribute("diffG")) info[5] = stof(pElement->Attribute("diffG"));
                if (pElement->Attribute("diffB")) info[6] = stof(pElement->Attribute("diffB"));

                info[7] = 1.0f;
            }

            if (pElement->Attribute("ambR") || pElement->Attribute("ambG") ||
                pElement->Attribute("ambB")) {
                attributes.push_back(AMBIENT);

                if (pElement->Attribute("ambR")) info[8] = stof(pElement->Attribute("ambR"));
                if (pElement->Attribute("ambG")) info[9] = stof(pElement->Attribute("ambG"));
                if (pElement->Attribute("ambB")) info[10] = stof(pElement->Attribute("ambB"));

                info[11] = 1.0f;
            }

            if (pElement->Attribute("specR") || pElement->Attribute("specG") ||
                pElement->Attribute("specB")) {
                attributes.push_back(SPECULAR);

                if (pElement->Attribute("specR")) info[12] = stof(pElement->Attribute("specR"));
                if (pElement->Attribute("specG")) info[13] = stof(pElement->Attribute("specG"));
                if (pElement->Attribute("specB")) info[14] = stof(pElement->Attribute("specB"));

                info[15] = 1.0f;
            }

            if (type != -1) {
                Light *light = new Light(info, attributes);
                scene->add_light(light);
            } else {
                free(info);
            }
        }
        pElement = pElement->NextSiblingElement("light");
    }
}

void parse_material(Shape *shape, XMLElement *element) {
    Transform *diffuse = nullptr;
    Transform *ambient = nullptr;
    Transform *specular = new Transform(0.0f, 0.0f, 0.0f);
    Transform *emission = new Transform(0.0f, 0.0f, 0.0f);

    // Diffuse
    if (element->Attribute("diffR") || element->Attribute("diffG") || element->Attribute("diffB")) {
        diffuse = new Transform(0.8f, 0.8f, 0.8f);

        if (element->Attribute("diffR")) diffuse->setX(stof(element->Attribute("diffR")));
        if (element->Attribute("diffG")) diffuse->setY(stof(element->Attribute("diffG")));
        if (element->Attribute("diffB")) diffuse->setZ(stof(element->Attribute("diffB")));
    }

    // Ambient
    if (element->Attribute("ambR") || element->Attribute("ambG") || element->Attribute("ambB")) {
        ambient = new Transform(0.2f, 0.2f, 0.2f);

        if (element->Attribute("ambR")) ambient->setX(stof(element->Attribute("ambR")));
        if (element->Attribute("ambG")) ambient->setY(stof(element->Attribute("ambG")));
        if (element->Attribute("ambB")) ambient->setZ(stof(element->Attribute("ambB")));
    }

    // Specular
    if (element->Attribute("specR")) specular->setX(stof(element->Attribute("specR")));
    if (element->Attribute("specG")) specular->setY(stof(element->Attribute("specG")));
    if (element->Attribute("specB")) specular->setZ(stof(element->Attribute("specB")));

    // Emission
    if (element->Attribute("emiR")) emission->setX(stof(element->Attribute("emiR")));
    if (element->Attribute("emiG")) emission->setY(stof(element->Attribute("emiG")));
    if (element->Attribute("emiB")) emission->setZ(stof(element->Attribute("emiB")));

    Material *m = new Material(diffuse, ambient, specular, emission);
    shape->setMaterial(m);
}
