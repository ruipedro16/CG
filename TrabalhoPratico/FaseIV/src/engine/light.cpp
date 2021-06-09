#include "light.hpp"

using std::vector;

#define POINT 1
#define DIRECTIONAL 2
#define DIFFUSE 3
#define AMBIENT 4
#define SPECULAR 5

Light::Light(float *info, const vector<int> &attributes) : info(info), attributes(attributes) {}

void Light::apply(GLenum num) {
    glLightfv(GL_LIGHT0 + num, GL_POSITION, info);  // set light source parameters

    for (int atr : attributes) {
        switch (atr) {
            case DIFFUSE:
                glLightfv(GL_LIGHT0 + num, GL_DIFFUSE, info + 4);
                break;
            case AMBIENT:
                glLightfv(GL_LIGHT0 + num, GL_AMBIENT, info + 8);
                break;
            case SPECULAR:
                glLightfv(GL_LIGHT0 + num, GL_SPECULAR, info + 12);
                break;
            default:
                break;
        }
    }
}