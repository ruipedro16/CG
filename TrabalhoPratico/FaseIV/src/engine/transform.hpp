#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <string>
#include <vector>

#include "../common/vertex.hpp"

class Transform {
private:
    std::string type;
    float angle, x, y, z, time;
    std::vector<Vertex *> controlPoints, pointsCurve;
    bool deriv;
    float vector[4] = {1, 0, 0};

public:
    Transform(const std::string &type, float angle, float x, float y, float z);

    Transform(const std::string &type, float time, const std::vector<Vertex *> &controlPoints,
              bool deriv);

    Transform(float x, float y, float z);

    const std::string &getType() const;

    float getAngle() const;

    float getX() const;

    void setX(float x);

    float getY() const;

    void setY(float y);

    void setZ(float z);

    float getZ() const;

    float getTime() const;

    const std::vector<Vertex *> &getPointsCurve() const;

    bool isDeriv() const;

    float *getVector();

    void normalize(float *v);

    void cross_product(float *a, float *b, float *res);

    void rotMatrix(float *r, float *xx, float *yy, float *zz);

    void multMatrixVector(float *m, float *v, float *res);

    void getCatmullRomPoint(float t, int *indexes, float *p, float *der);

    void getGlobalCatmullRomPoint(float gt, float *p, float *der);

    void setCatmullPoints();
};

#endif
