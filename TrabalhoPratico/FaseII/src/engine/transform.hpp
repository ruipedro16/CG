#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <string>

class Transform {
private:
    std::string type;
    float angle, x, y, z;

public:
    Transform(const std::string &type, float angle, float x, float y, float z);

    const std::string &getType() const;

    float getAngle() const;

    float getX() const;

    float getY() const;

    float getZ() const;
};

#endif