#include "transform.hpp"

using namespace std;

Transform::Transform(const string &type, float angle, float x, float y, float z)
    : type(type), angle(angle), x(x), y(y), z(z) {}

const string &Transform::getType() const { return type; }

float Transform::getAngle() const { return angle; }

float Transform::getX() const { return x; }

float Transform::getY() const { return y; }

float Transform::getZ() const { return z; }
