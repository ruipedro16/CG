#include "transform.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

Transform::Transform(const std::string &type, float angle, float x, float y,
                     float z)
    : type(type), angle(angle), x(x), y(y), z(z) {}

Transform::Transform(const std::string &type, float time,
                     const std::vector<Vertex *> &controlPoints, bool deriv)
    : type(type), time(time), controlPoints(controlPoints), deriv(deriv) {
  setCatmullPoints();
}

const std::string &Transform::getType() const { return type; }

float Transform::getAngle() const { return angle; }

float Transform::getX() const { return x; }

float Transform::getY() const { return y; }

float Transform::getZ() const { return z; }

float Transform::getTime() const { return time; }

const std::vector<Vertex *> &Transform::getPointsCurve() const {
  return pointsCurve;
}

bool Transform::isDeriv() const { return deriv; }

float *Transform::getVector() { return vector; }

void Transform::normalize(float *v) {
  float n = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  v[0] = v[0] / n;
  v[1] = v[1] / n;
  v[2] = v[2] / n;
}

void Transform::cross_product(float *a, float *b, float *res) {
  res[0] = a[1] * b[2] - a[2] * b[1];
  res[1] = a[2] * b[0] - a[0] * b[2];
  res[2] = a[0] * b[1] - a[1] * b[0];
}

void Transform::rotMatrix(float *r, float *xx, float *yy, float *zz) {
  r[0] = xx[0];
  r[1] = xx[1];
  r[2] = xx[2];
  r[3] = 0.0f;
  r[4] = yy[0];
  r[5] = yy[1];
  r[6] = yy[2];
  r[7] = 0.0f;
  r[8] = zz[0];
  r[9] = zz[1];
  r[10] = zz[2];
  r[11] = 0.0f;
  r[12] = 0.0f;
  r[13] = 0.0f;
  r[14] = 0.0f;
  r[15] = 1.0f;
}

void Transform::multMatrixVector(float *m, float *v, float *res) {
  for (size_t i = 0; i < 4; ++i) {
    res[i] = 0;
    for (size_t j = 0; j < 4; ++j)
      res[i] += v[j] * m[i * 4 + j];
  }
}

void Transform::getCatmullRomPoint(float t, int *indexes, float *p,
                                   float *der) {
  float m[4][4] = {{-0.5f, 1.5f, -1.5f, 0.5f},
                   {1.0f, -2.5f, 2.0f, -0.5f},
                   {-0.5f, 0.0f, 0.5f, 0.0f},
                   {0.0f, 1.0f, 0.0f, 0.0f}};

  float px[4], py[4], pz[4];
  for (size_t i = 0; i < 4; i++) {
    px[i] = controlPoints[indexes[i]]->getX();
    py[i] = controlPoints[indexes[i]]->getY();
    pz[i] = controlPoints[indexes[i]]->getZ();
  }

  float a[4][4];
  multMatrixVector(*m, px, a[0]);
  multMatrixVector(*m, py, a[1]);
  multMatrixVector(*m, pz, a[2]);

  float T[4] = {t * t * t, t * t, t, 1};
  multMatrixVector(*a, T, p);

  float Tdev[4] = {3 * T[1], 2 * T[2], 1, 0};
  multMatrixVector(*a, Tdev, der);
}

void Transform::getGlobalCatmullRomPoint(float gt, float *p, float *der) {
  unsigned int num = controlPoints.size();
  float t = gt * (float)num;
  int index = floor(t);

  t = t - (float)index;

  int indexes[4];
  indexes[0] = (index + num - 1) % num;
  indexes[1] = (indexes[0] + 1) % num;
  indexes[2] = (indexes[1] + 1) % num;
  indexes[3] = (indexes[2] + 1) % num;

  getCatmullRomPoint(t, indexes, p, der);
}

void Transform::setCatmullPoints() {
  float ponto[4];
  float der[4];

  for (float i = 0; i < 1; i += 0.01) {
    getGlobalCatmullRomPoint(i, ponto, der);
    Vertex *p = new Vertex(ponto[0], ponto[1], ponto[2]);
    pointsCurve.push_back(p);
  }
}
