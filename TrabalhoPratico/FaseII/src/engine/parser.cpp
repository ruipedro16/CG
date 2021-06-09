#include "parser.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int read_file(const char *path, vector<Vertex *> *vertices) {
    ifstream file(path);
    if (!file.is_open()) {
        cout << "Could not open file " << path << endl;
        return 1;
    }
    float x, y, z;
    while (file >> x >> y >> z) vertices->push_back(new Vertex(x, y, z));
    file.close();
    return 0;
}

Group *load_XML_file(const char *path, vector<Vertex *> *vertices) {
    Group *group = nullptr;
    XMLDocument xmlDoc;
    XMLNode *pNode;
    XMLElement *pElement;

    if (xmlDoc.LoadFile(path) == XML_SUCCESS) {
        pNode = xmlDoc.FirstChild();

        if (pNode != nullptr) {
            group = new Group();
            pElement = pNode->FirstChildElement("group");
            parse_group(group, pElement, vertices, 0);
        }
    } else
        cout << "Could not open file " << path << endl;

    return group;
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

    if (element->Attribute("angle")) angle = stof(element->Attribute("angle"));
    if (element->Attribute("X")) x = stof(element->Attribute("X"));
    if (element->Attribute("Y")) y = stof(element->Attribute("Y"));
    if (element->Attribute("Z")) z = stof(element->Attribute("Z"));

    group->addTransform(new Transform(type, angle, x, y, z));
}

void parse_translate(Group *group, XMLElement *element, vector<Vertex *> *vertices, int d) {
    float x = 0.0f, y = 0.0f, z = 0.0f;
    string type = "translation";

    if (element->Attribute("X")) x = stof(element->Attribute("X"));
    if (element->Attribute("Y")) y = stof(element->Attribute("Y"));
    if (element->Attribute("Z")) z = stof(element->Attribute("Z"));

    group->addTransform(new Transform(type, 0, x, y, z));

    if (d == 0 || d == 1) vertices->push_back(new Vertex(x, y, z));
}

void parse_colour(Group *group, XMLElement *element) {
    float x = 1.0f, y = 1.0f, z = 1.0f;
    string type = "colour";

    if (element->Attribute("R")) x = stof(element->Attribute("R"));
    if (element->Attribute("G")) y = stof(element->Attribute("G"));
    if (element->Attribute("B")) z = stof(element->Attribute("B"));

    group->addTransform(new Transform(type, 0.0f, x, y, z));
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
            read_file(file_path.c_str(), &vertices);
            if (vertices.size()) {
                Shape *shape = new Shape(vertices);
                shapes.push_back(shape);
            }
        }

        element = element->NextSiblingElement("model");
    }

    if (shapes.size()) group->setShapes(shapes);
}

void parse_group(Group *group, XMLElement *gElement, vector<Vertex *> *vertices, int d) {
    XMLElement *element = gElement->FirstChildElement();

    while (element) {
        if (!strcmp(element->Name(), "translate"))
            parse_translate(group, element, vertices, d);

        else if (!strcmp(element->Name(), "scale"))
            parse_scale(group, element);

        else if (!strcmp(element->Name(), "rotate"))
            parse_rotate(group, element);

        else if (!strcmp(element->Name(), "models"))
            parse_models(group, element);

        else if (!strcmp(element->Name(), "colour"))
            parse_colour(group, element);

        else if (!strcmp(element->Name(), "group")) {
            Group *child = new Group();
            group->addGroup(child);
            parse_group(child, element, vertices, d + 1);
        }

        element = element->NextSiblingElement();
    }
}