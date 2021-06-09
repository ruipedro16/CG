#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "transform.hpp"

class Material {
private:
    float diffuse[4];   // default: {0.8f, 0.8f, 0.8f, 1.0f};
    float ambient[4];   // default: {0.2f, 0.2f, 0.2f, 1.0f};
    float specular[4];  // default: {0.0f, 0.0f, 0.0f, 1.0f};
    float emission[4];  // default: {0.0f, 0.0f, 0.0f, 1.0f};
public:
    Material(Transform *dif, Transform *amb, Transform *spec, Transform *em);
    void draw();
};

#endif
