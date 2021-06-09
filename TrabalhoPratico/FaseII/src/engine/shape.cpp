#include "shape.hpp"

using namespace std;

Shape::Shape(const vector<Vertex *> &vertices) : vertices(vertices) {}

const vector<Vertex *> &Shape::getVertices() const { return vertices; }
