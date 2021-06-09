#ifndef __VERTEX_H__
#define __VERTEX_H__

#include <string>

class Vertex {
private:
    float x, y, z;

public:
    Vertex(float x, float y, float z);

    float getX() const;

    float getY() const;

    float getZ() const;

    std::string to_string() const;
};

#endif
