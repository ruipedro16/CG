#ifndef __LIGHT_H__
#define __LIGHT_H__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <vector>

class Light {
private:
    float *info;
    std::vector<int> attributes;

public:
    Light(float *info, const std::vector<int> &attributes);
    void apply(GLenum num);
};

#endif
