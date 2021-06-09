#include "vertex.hpp"

#include <sstream>

Vertex::Vertex(float x, float y, float z) : x(x), y(y), z(z) {}

float Vertex::getX() const { return x; }

float Vertex::getY() const { return y; }

float Vertex::getZ() const { return z; }

std::string Vertex::to_string() const {
    std::ostringstream sstream;
    sstream << x << ' ' << y << ' ' << z;
    return sstream.str();
}
