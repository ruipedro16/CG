#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <vector>

#include "../common/vertex.hpp"

class Shape {
private:
    std::vector<Vertex *> vertices;

public:
    Shape(const std::vector<Vertex *> &vertices);

    const std::vector<Vertex *> &getVertices() const;
};

#endif