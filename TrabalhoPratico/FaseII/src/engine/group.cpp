#include "group.hpp"

using namespace std;

Group::Group() {}

const vector<Group *> &Group::getGroups() const { return groups; }

const vector<Transform *> &Group::getTransforms() const { return transforms; }

const vector<Shape *> &Group::getShapes() const { return shapes; }

void Group::setShapes(const std::vector<Shape *> &shapes) { Group::shapes = shapes; }

void Group::addGroup(Group *g) { groups.push_back(g); }

void Group::addTransform(Transform *t) { transforms.push_back(t); }
