#ifndef __GROUP_H__
#define __GROUP_H__

#include <vector>

#include "shape.hpp"
#include "transform.hpp"

class Group {
private:
    std::vector<Group *> groups;
    std::vector<Transform *> transforms;
    std::vector<Shape *> shapes;

public:
    Group();

    const std::vector<Group *> &getGroups() const;

    const std::vector<Transform *> &getTransforms() const;

    const std::vector<Shape *> &getShapes() const;

    void setShapes(const std::vector<Shape *> &shapes);

    void addGroup(Group *g);

    void addTransform(Transform *t);
};

#endif
