#include "shape.hpp"

using namespace std;

Shape::Shape(const vector<Vertex *> &vertices) {
  nVertices = vertices.size();
  float *vert = (float *)malloc(nVertices * 3 * sizeof(float));

  size_t i = 0;
  for (vector<Vertex *>::const_iterator v_it = vertices.begin();
       v_it != vertices.end(); ++v_it) {
    vert[i++] = (*v_it)->getX();
    vert[i++] = (*v_it)->getY();
    vert[i++] = (*v_it)->getZ();
  }

  glGenBuffers(1, buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nVertices * 3, vert,
               GL_STATIC_DRAW);

  free(vert);
}

void Shape::drawShape() {
  glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  glDrawArrays(GL_TRIANGLES, 0, nVertices);
}
