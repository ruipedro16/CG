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
    float m[4][4] = {{-1.0f, 3.0f, -3.0f, 1.0f},
                     {3.0f, -6.0f, 3.0f, 0.0f},
                     {-3.0f, 3.0f, 0.0f, 0.0f},
                     {1.0f, 0.0f, 0.0f, 0.0f}};
    Patch(std::vector<Vertex> p);
    Patch(int tess, const char *file);
    void parsePatchFile(const char *filename);
    void multMatrixVector(float *m, float *v, float *res);
    Vertex *getPoint(float ta, float tb, float coordX[4][4], float coordY[4][4],
                     float coordZ[4][4]);
    float *getTangent(float tu, float tv, float mX[4][4], float mY[4][4], float mZ[4][4], int type);
    void normalize(float *a);
    void cross(float *a, float *b, float *res);
    void getPatchPoints(int patch, std::vector<Vertex> *points, std::vector<float> *texture,
                        std::vector<Vertex> *normal);
    void BezierModelGenerator(std::vector<Vertex> *vert, std::vector<Vertex> *normal,
                              std::vector<float> *texture);
};

#endif
