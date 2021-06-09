#ifndef __PATCH_H__
#define __PATCH_H__

#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "../common/vertex.hpp"

class Patch {
private:
  int nPatches, nPoints, tesselation;
  std::vector<Vertex> controlPoints;
  std::map<int, std::vector<int>> patches;

public:
  Patch(std::vector<Vertex> p);
  Patch(int tess, const char *file);
  void parsePatchFile(const char *filename);
  void multMatrixVector(float *m, float *v, float *res);
  Vertex *getPoint(float ta, float tb, float coordX[4][4], float coordY[4][4],
                   float coordZ[4][4]);
  std::vector<Vertex> getPatchPoints(int patch);
  std::vector<Vertex> BezierModelGenerator();
};

#endif
