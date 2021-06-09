#include "vertex.hpp"

#include <sstream>

Vertex::Vertex() {}

Vertex::Vertex(float x, float y, float z) : x(x), y(y), z(z) {}

float Vertex::getX() const { return x; }

float Vertex::getY() const { return y; }

float Vertex::getZ() const { return z; }

void Vertex::setX(float x) { this->x = x; }

void Vertex::setY(float y) { this->y = y; }

void Vertex::setZ(float z) { this->z = z; }

std::string Vertex::to_string() const {
    std::ostringstream sstream;
    sstream << x << ' ' << y << ' ' << z;
    return sstream.str();
}
