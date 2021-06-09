#include "material.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

Material::Material(Transform *dif, Transform *amb, Transform *spec, Transform *em) {
    diffuse[3] = -1;
    ambient[3] = -1;

    if (dif) {
        diffuse[0] = dif->getX();
        diffuse[1] = dif->getY();
        diffuse[2] = dif->getZ();
        diffuse[3] = 1;
    }

    if (amb) {
        ambient[0] = amb->getX();
        ambient[1] = amb->getY();
        ambient[2] = amb->getZ();
        ambient[3] = 1;
    }

    specular[0] = spec->getX();
    specular[1] = spec->getY();
    specular[2] = spec->getZ();
    specular[3] = 1;

    emission[0] = em->getX();
    emission[1] = em->getY();
    emission[2] = em->getZ();
    emission[3] = 1;
}

void Material::draw() {
    if (diffuse[3] != -1) glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

    if (ambient[3] != -1) glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
}
