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

class Shape {
private:
  unsigned int nVertices;
  GLuint buffer[1];

public:
  Shape(const std::vector<Vertex *> &vertices);
  void drawShape();
};

#endif
