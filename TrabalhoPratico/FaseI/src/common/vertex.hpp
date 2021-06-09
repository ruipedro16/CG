#ifndef VERTEX_H
#define VERTEX_H

#include <string>

class Vertex {
   private:
    float _x, _y, _z;

   public:
    Vertex(float, float, float);
    std::string to_string() const;
    float x() const { return _x; }
    float y() const { return _y; }
    float z() const { return _z; }
};

#endif
