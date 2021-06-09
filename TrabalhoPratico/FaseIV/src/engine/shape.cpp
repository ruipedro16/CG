#include "shape.hpp"

#include <IL/il.h>

#include <iostream>

#include "material.hpp"

using std::vector;

Shape::Shape(const std::vector<Vertex *> &vert, const std::vector<Vertex *> &normal,
             const std::vector<float> &text) {
    nVertices[0] = vert.size();
    nVertices[1] = normal.size();
    nVertices[2] = text.size();
    prepare_buffer(vert, normal, text);
}

Shape::Shape(const char *filepath, const std::vector<Vertex *> &vert,
             const std::vector<Vertex *> &normal, const std::vector<float> &text) {
    nVertices[0] = vert.size();
    nVertices[1] = normal.size();
    nVertices[2] = text.size();
    prepare_buffer(vert, normal, text);
    loadTexture(filepath);
}

void Shape::setMaterial(Material *materials) { this->materials = materials; }

void Shape::prepare_buffer(const std::vector<Vertex *> &vert, const std::vector<Vertex *> &normal,
                           const std::vector<float> &text) {
    size_t i = 0;
    float *vertices = new float[vert.size() * 3];
    for (vector<Vertex *>::const_iterator vertex_it = vert.begin(); vertex_it != vert.end();
         ++vertex_it) {
        vertices[i++] = (*vertex_it)->getX();
        vertices[i++] = (*vertex_it)->getY();
        vertices[i++] = (*vertex_it)->getZ();
    }

    i = 0;
    float *normals = new float[normal.size() * 3];
    for (vector<Vertex *>::const_iterator vertex_it = normal.begin(); vertex_it != normal.end();
         ++vertex_it) {
        normals[i++] = (*vertex_it)->getX();
        normals[i++] = (*vertex_it)->getY();
        normals[i++] = (*vertex_it)->getZ();
    }

    glGenBuffers(3, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nVertices[0] * 3, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nVertices[1] * 3, normals, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nVertices[2], &(text[0]), GL_STATIC_DRAW);

    free(vertices);
    free(normals);
}

void Shape::loadTexture(const char *filepath) {
    unsigned int t, tw, th;
    unsigned char *textData;

    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring)filepath);
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    textData = ilGetData();

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, textData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Shape::drawShape() {
    materials->draw();

    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    if (nVertices[1] > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
        glNormalPointer(GL_FLOAT, 0, 0);
    }

    if (nVertices[2] > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    glDrawArrays(GL_TRIANGLES, 0, nVertices[0] * 3);
    glBindTexture(GL_TEXTURE_2D, 0);
}
