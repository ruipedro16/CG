#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>

#include "../common/vertex.hpp"
#include "group.hpp"
#include "shape.hpp"
#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

Group *load_XML_file(const char *path, std::vector<Vertex *> *vertices);

void parse_group(Group *group, XMLElement *gElement, std::vector<Vertex *> *vertices, int d);

#endif