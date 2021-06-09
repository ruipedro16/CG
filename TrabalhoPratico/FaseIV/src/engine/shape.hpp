#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "../common/vertex.hpp"
#include "material.hpp"

class Shape {
private:
    unsigned int nVertices[3];
    GLuint buffer[3];
    GLuint texture;
    Material *materials;

public:
    Shape(const std::vector<Vertex *> &vert, const std::vector<Vertex *> &normal,
          const std::vector<float> &text);
    Shape(const char *filepath, const std::vector<Vertex *> &vert,
          const std::vector<Vertex *> &normal, const std::vector<float> &text);
    void setMaterial(Material *mat);
    void prepare_buffer(const std::vector<Vertex *> &vert, const std::vector<Vertex *> &normal,
                        const std::vector<float> &text);
    void loadTexture(const char *filepath);
    void drawShape();
};

#endif
