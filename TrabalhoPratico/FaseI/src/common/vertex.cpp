#include "vertex.hpp"

#include <sstream>

Vertex::Vertex(float x, float y, float z) : _x(x), _y(y), _z(z) {}

std::string Vertex::to_string() const {
    std::ostringstream sstream;
    sstream << _x << ' ' << _y << ' ' << _z;
    return sstream.str();
}
