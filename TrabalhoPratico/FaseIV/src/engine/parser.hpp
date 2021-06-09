#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>

#include "../common/vertex.hpp"
#include "group.hpp"
#include "scene.hpp"
#include "shape.hpp"
#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

Scene *load_XML_file(const char *path);

void parse_group(Scene *scene, Group *group, XMLElement *gElement);

void parse_lights(Scene *scene, XMLElement *pElement);

void parse_material(Shape *shape, XMLElement *element);

#endif
