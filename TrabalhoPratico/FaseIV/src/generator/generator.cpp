#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "../common/vertex.hpp"
#include "patch.hpp"

using namespace std;

// Global Variables
float quadrants[8][3] = {
    {1, 1, 1},  {1, 1, -1},  {-1, 1, -1},  {-1, 1, 1},
    {1, -1, 1}, {1, -1, -1}, {-1, -1, -1}, {-1, -1, 1},
};

float normals[6][3] = {{0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}, {1, 0, 0}, {-1, 0, 0}};

float texturePlane[4][2] = {{1, 0}, {1, 1}, {0, 1}, {0, 0}};

int write_file(const char *filepath, vector<Vertex> *vertices, vector<Vertex> *normals,
               vector<float> *textures) {
    ofstream file(filepath);
    if (!file.is_open()) {
        cout << "Could not open file " << filepath << endl;
        return 1;
    }
    file << vertices->size() << endl;
    for (vector<Vertex>::const_iterator it = vertices->begin(); it != vertices->end(); ++it) {
        file << (*it).to_string() << endl;
    }
    file << normals->size() << endl;
    for (vector<Vertex>::const_iterator it = normals->begin(); it != normals->end(); ++it) {
        file << (*it).to_string() << endl;
    }
    file << textures->size() / 2 << endl;
    for (size_t i = 0; i < textures->size(); i += 2) {
        file << textures->at(i) << ' ' << textures->at(i + 1) << endl;
    }
    file.close();
    return 0;
}

void help() {
    cout << "Usage: ./generator <shape> [options] <file>" << endl;
    cout << "\nShapes & Options:" << endl;
    cout << "\tplane <size>" << endl;
    cout << "\tbox <width> <height> <length>" << endl;
    cout << "\tbox <width> <height> <length> <divisions>" << endl;
    cout << "\tsphere <radius> <slices> <stacks>" << endl;
    cout << "\tcone <radius> <height> <slices> <stacks>" << endl;
    cout << "\t-> For an inverted cone, use a negative height" << endl;
    cout << "\ttorus <inner radius> <outer radius> <slices> <stacks>" << endl;
    cout << "\tpatch <tesselation> <patch file>" << endl;
}

Vertex makeVertex(float radius, float angle, float height) {
    return Vertex(radius * sin(angle), height, radius * cos(angle));
}

Vertex makeVertexSphere(float radius, float beta, float alpha) {
    return Vertex(radius * sin(alpha) * cos(beta), radius * sin(beta),
                  radius * cos(beta) * cos(alpha));
}

Vertex makeNormalVertex(float angle, float teta) {
    return Vertex(cos(angle) * sin(teta), sin(angle), cos(angle) * cos(teta));
}

vector<Vertex> generate_plane(float side_length, vector<Vertex> *normal, vector<float> *texture) {
    vector<Vertex> vertices;
    Vertex p;
    side_length = side_length / 2;
    int face[6] = {0, 1, 3, 3, 1, 2};

    for (int i = 0; i < 6; i++) {
        int tmp = face[i];
        p.setX(side_length * quadrants[tmp][0]);
        p.setY(0.0f);
        p.setZ(side_length * quadrants[tmp][2]);
        vertices.push_back(p);

        p.setX(0.0f);
        p.setY(1.0f);
        p.setZ(0.0f);
        (*normal).push_back(p);

        (*texture).push_back(texturePlane[tmp][0]);
        (*texture).push_back(texturePlane[tmp][0]);
    }

    return vertices;
}

void generate_box_sides(float x, float y, float z, int divis, vector<Vertex> *vertices,
                        vector<Vertex> *normal, vector<float> *texture) {
    float d_z = z / (float)(divis);
    float d_y = y / (float)(divis);
    float texZ_inv = 0.75f, texZ = 0.0f, texY = 0.666667f;
    float divTexZ = 0.25f / (float)(divis), divTexY = 0.333333f / (float)(divis);
    float x2 = x / 2;
    Vertex p1, p2, p3, p4, p5, p6;

    for (float i = y / 2; i > -y / 2; i -= d_y) {
        for (float j = -z / 2; j < z / 2; j += d_z) {
            p1.setX(x2);
            p1.setY(i);
            p1.setZ(j);

            p2.setX(x2);
            p2.setY(i - d_y);
            p2.setZ(j);

            p3.setX(x2);
            p3.setY(i - d_y);
            p3.setZ(j + d_z);

            p4.setX(x2);
            p4.setY(i);
            p4.setZ(j + d_z);

            p5.setX(1.0f);
            p5.setY(0.0f);
            p5.setZ(0.0f);
            p6.setX(-1.0f);
            p6.setY(0.0f);
            p6.setZ(0.0f);

            vertices->push_back(p4);
            vertices->push_back(p3);
            vertices->push_back(p2);

            vertices->push_back(p2);
            vertices->push_back(p1);
            vertices->push_back(p4);

            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);

            (*texture).push_back(texZ_inv - divTexZ);
            (*texture).push_back(texY);
            (*texture).push_back(texZ_inv - divTexZ);
            (*texture).push_back(texY - divTexY);
            (*texture).push_back(texZ_inv);
            (*texture).push_back(texY - divTexY);
            (*texture).push_back(texZ_inv);
            (*texture).push_back(texY - divTexY);
            (*texture).push_back(texZ_inv);
            (*texture).push_back(texY);
            (*texture).push_back(texZ_inv - divTexZ);
            (*texture).push_back(texY);

            p1.setX(-x2);
            p2.setX(-x2);
            p3.setX(-x2);
            p4.setX(-x2);

            vertices->push_back(p1);
            vertices->push_back(p2);
            vertices->push_back(p3);

            vertices->push_back(p3);
            vertices->push_back(p4);
            vertices->push_back(p1);

            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);

            (*texture).push_back(texZ);
            (*texture).push_back(texY);
            (*texture).push_back(texZ);
            (*texture).push_back(texY - divTexY);
            (*texture).push_back(texZ + divTexZ);
            (*texture).push_back(texY - divTexY);
            (*texture).push_back(texZ + divTexZ);
            (*texture).push_back(texY - divTexY);
            (*texture).push_back(texZ + divTexZ);
            (*texture).push_back(texY);
            (*texture).push_back(texZ);
            (*texture).push_back(texY);

            texZ_inv -= divTexZ;
            texZ += divTexZ;
        }
        texY -= divTexY;
        texZ = 0.0f;
        texZ_inv = 0.75f;
    }
}

void generate_box_top_bottom(float x, float y, float z, int divis, vector<Vertex> *vertices,
                             vector<Vertex> *normal, vector<float> *texture) {
    float d_x = x / (float)(divis);
    float d_z = z / (float)(divis);
    float texX_inv = 0.5f, texX = 0.5f, texZTop = 1.0f, texZBot = 0.333333f;
    float divTexX = 0.25f / (float)(divis);
    float divTexZ = 0.333333f / (float)(divis);
    float y2 = y / 2;
    Vertex p1, p2, p3, p4, p5, p6;

    for (float i = z / 2; i > -z / 2; i -= d_z) {
        for (float j = -x / 2; j < x / 2; j += d_x) {
            /*    p2---p3
             *     | \ |
             *    p1---p4
             */

            p1.setX(j);
            p1.setY(y2);
            p1.setZ(i);

            p2.setX(j);
            p2.setY(y2);
            p2.setZ(i - d_z);

            p3.setX(j + d_x);
            p3.setY(y2);
            p3.setZ(i - d_z);

            p4.setX(j + d_x);
            p4.setY(y2);
            p4.setZ(i);

            p5.setX(0.0f);
            p5.setY(1.0f);
            p5.setZ(0.0f);
            p6.setX(0.0f);
            p6.setY(-1.0f);
            p6.setZ(0.0f);

            vertices->push_back(p4);
            vertices->push_back(p3);
            vertices->push_back(p2);

            vertices->push_back(p2);
            vertices->push_back(p1);
            vertices->push_back(p4);

            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);

            (*texture).push_back(texX_inv - divTexX);
            (*texture).push_back(texZTop);
            (*texture).push_back(texX_inv - divTexX);
            (*texture).push_back(texZTop - divTexZ);
            (*texture).push_back(texX_inv);
            (*texture).push_back(texZTop - divTexZ);
            (*texture).push_back(texX_inv);
            (*texture).push_back(texZTop - divTexZ);
            (*texture).push_back(texX_inv);
            (*texture).push_back(texZTop);
            (*texture).push_back(texX_inv - divTexX);
            (*texture).push_back(texZTop);

            p1.setY(-y2);
            p2.setY(-y2);
            p3.setY(-y2);
            p4.setY(-y2);

            vertices->push_back(p1);
            vertices->push_back(p2);
            vertices->push_back(p3);

            vertices->push_back(p3);
            vertices->push_back(p4);
            vertices->push_back(p1);

            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);

            (*texture).push_back(texX);
            (*texture).push_back(texZBot);
            (*texture).push_back(texX);
            (*texture).push_back(texZBot - divTexZ);
            (*texture).push_back(texX - divTexX);
            (*texture).push_back(texZBot - divTexZ);
            (*texture).push_back(texX - divTexX);
            (*texture).push_back(texZBot - divTexZ);
            (*texture).push_back(texX - divTexX);
            (*texture).push_back(texZBot);
            (*texture).push_back(texX);
            (*texture).push_back(texZBot);

            texX_inv -= divTexX;
            texX -= divTexX;
        }
        texZTop -= divTexZ;
        texZBot -= divTexZ;
        texX_inv = 0.5f;
        texX = 0.5f;
    }
}

void generate_box_front(float x, float y, float z, int divis, vector<Vertex> *vertices,
                        vector<Vertex> *normal, vector<float> *texture) {
    float d_x = x / (float)(divis);
    float d_y = y / (float)(divis);
    float texX_inv = 1.0f, texX = 0.25f, texY = 0.666667f;
    float divTexX = 0.25f / (float)(divis), divTexY = 0.333333f / (float)(divis);
    float z2 = z / 2;
    Vertex p1, p2, p3, p4, p5, p6;

    for (float i = y / 2; i > -y / 2; i -= d_y) {
        for (float j = -x / 2; j < x / 2; j += d_x) {
            /*
             * p1---p4
             *  | \ |
             * p2---p3
             */
            p1.setX(j);
            p1.setY(i);
            p1.setZ(z2);

            p2.setX(j);
            p2.setY(i - d_y);
            p2.setZ(z2);

            p3.setX(j + d_x);
            p3.setY(i - d_y);
            p3.setZ(z2);

            p4.setX(j + d_x);
            p4.setY(i);
            p4.setZ(z2);

            p5.setX(0.0f);
            p5.setY(0.0f);
            p5.setZ(1.0f);
            p6.setX(0.0f);
            p6.setY(0.0f);
            p6.setZ(-1.0f);

            vertices->push_back(p1);
            vertices->push_back(p2);
            vertices->push_back(p3);

            vertices->push_back(p3);
            vertices->push_back(p4);
            vertices->push_back(p1);

            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);

            (*texture).push_back(texX);
            (*texture).push_back(texY);
            (*texture).push_back(texX);
            (*texture).push_back(texY - divTexY);
            (*texture).push_back(texX + divTexX);
            (*texture).push_back(texY - divTexY);
            (*texture).push_back(texX + divTexX);
            (*texture).push_back(texY - divTexY);
            (*texture).push_back(texX + divTexX);
            (*texture).push_back(texY);
            (*texture).push_back(texX);
            (*texture).push_back(texY);

            p1.setZ(-z2);

            p2.setZ(-z2);

            p3.setZ(-z2);

            p4.setZ(-z2);

            vertices->push_back(p4);
            vertices->push_back(p3);
            vertices->push_back(p2);

            vertices->push_back(p2);
            vertices->push_back(p1);
            vertices->push_back(p4);

            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);

            (*texture).push_back(texX_inv - divTexX);
            (*texture).push_back(texY);
            (*texture).push_back(texX_inv - divTexX);
            (*texture).push_back(texY - divTexY);
            (*texture).push_back(texX_inv);
            (*texture).push_back(texY - divTexY);
            (*texture).push_back(texX_inv);
            (*texture).push_back(texY - divTexY);
            (*texture).push_back(texX_inv);
            (*texture).push_back(texY);
            (*texture).push_back(texX_inv - divTexX);
            (*texture).push_back(texY);

            texX += divTexX;
            texX_inv -= divTexX;
        }
        texY -= divTexY;
        texX = 0.25f;
        texX_inv = 1.0f;
    }
}

vector<Vertex> generate_box(float x, float y, float z, int divis, vector<Vertex> *normal,
                            vector<float> *texture) {
    vector<Vertex> vertices;
    generate_box_sides(x, y, z, divis, &vertices, normal, texture);
    generate_box_top_bottom(x, y, z, divis, &vertices, normal, texture);
    generate_box_front(x, y, z, divis, &vertices, normal, texture);
    return vertices;
}

vector<Vertex> generate_sphere(float radius, int slices, int stacks, vector<Vertex> *normal,
                               vector<float> *texture) {
    float alpha1, alpha2, beta1, beta2;
    vector<Vertex> vertices;
    Vertex p1, p2, p3, p4;

    for (int i = 0; i < stacks; i++) {
        beta1 = (float)i * (float)(M_PI / stacks) - (float)M_PI_2;
        beta2 = (float)(i + 1) * (float)(M_PI / stacks) - (float)M_PI_2;
        for (int j = 0; j < slices; j++) {
            alpha1 = (float)j * 2 * (float)M_PI / (float)slices;
            alpha2 = (float)(j + 1) * 2 * (float)M_PI / (float)slices;

            p1 = makeVertexSphere(radius, beta2, alpha1);
            p2 = makeVertexSphere(radius, beta1, alpha1);
            p3 = makeVertexSphere(radius, beta2, alpha2);
            p4 = makeVertexSphere(radius, beta1, alpha2);

            vertices.push_back(p1);
            vertices.push_back(p2);
            vertices.push_back(p3);

            vertices.push_back(p3);
            vertices.push_back(p2);
            vertices.push_back(p4);

            p1 = makeVertexSphere(1, beta2, alpha1);
            p2 = makeVertexSphere(1, beta1, alpha1);
            p3 = makeVertexSphere(1, beta2, alpha2);
            p4 = makeVertexSphere(1, beta1, alpha2);

            (*normal).push_back(p1);
            (*normal).push_back(p2);
            (*normal).push_back(p3);
            (*normal).push_back(p3);
            (*normal).push_back(p2);
            (*normal).push_back(p4);

            (*texture).push_back((float)j / (float)slices);
            (*texture).push_back((float)(i + 1) / (float)stacks);

            (*texture).push_back((float)j / (float)slices);
            (*texture).push_back((float)i / (float)stacks);

            (*texture).push_back((float)(j + 1) / (float)slices);
            (*texture).push_back((float)(i + 1) / (float)stacks);

            (*texture).push_back((float)(j + 1) / (float)slices);
            (*texture).push_back((float)(i + 1) / (float)stacks);

            (*texture).push_back((float)j / (float)slices);
            (*texture).push_back((float)i / (float)stacks);
            
            (*texture).push_back((float)(j + 1) / (float)slices);
            (*texture).push_back((float)i / (float)stacks);
        }
    }
    return vertices;
}

vector<Vertex> generate_cone(float radius, float height, int slices, int stacks,
                             vector<Vertex> *normal, vector<float> *texture) {
    float alpha, scaleH, scaleR, radiusNow, radiusNext, currentHeight, heightNext, theta, thetaNext,
        angle, res, resNext;
    vector<Vertex> vertices;
    Vertex p1, p2, p3, p4;

    alpha = (float)(2 * M_PI) / (float)slices;
    scaleH = height / (float)stacks;
    scaleR = radius / (float)stacks;

    p1.setX(0.0f);
    p1.setY(0.0f);
    p1.setZ(0.0f);

    p4.setX(0.0f);
    p4.setY(-1.0f);
    p4.setZ(0.0f);

    // Base
    for (int i = 0; i < slices; i++) {
        theta = (float)i * alpha;
        thetaNext = (float)(i + 1) * alpha;

        p2 = makeVertex(radius, thetaNext, 0);
        p3 = makeVertex(radius, theta, 0);

        vertices.push_back(p1);
        vertices.push_back(p2);
        vertices.push_back(p3);

        (*normal).push_back(p4);
        (*normal).push_back(p4);
        (*normal).push_back(p4);

        (*texture).push_back(0.25f);
        (*texture).push_back(0.5f);
        (*texture).push_back(0.25f + cos(thetaNext) / 4.0f);
        (*texture).push_back(0.5f + sin(thetaNext) / 2.0f);
        (*texture).push_back(0.25f + cos(theta) / 4.0f);
        (*texture).push_back(0.5f + sin(theta) / 2.0f);
    }

    angle = atan(radius / height);

    // Body
    for (int i = 0; i < stacks; i++) {
        currentHeight = (float)i * scaleH;
        heightNext = (float)(i + 1) * scaleH;
        radiusNow = radius - (float)i * scaleR;
        radiusNext = radius - (float)(i + 1) * scaleR;

        for (int j = 0; j < slices; j++) {
            theta = (float)j * alpha;
            thetaNext = (float)(j + 1) * alpha;

            p1 = makeVertex(radiusNow, theta, currentHeight);
            p2 = makeVertex(radiusNow, thetaNext, currentHeight);
            p3 = makeVertex(radiusNext, thetaNext, heightNext);
            p4 = makeVertex(radiusNext, theta, heightNext);

            vertices.push_back(p1);
            vertices.push_back(p2);
            vertices.push_back(p3);

            vertices.push_back(p1);
            vertices.push_back(p3);
            vertices.push_back(p4);

            p1 = makeNormalVertex(angle, theta);
            p2 = makeNormalVertex(angle, thetaNext);

            (*normal).push_back(p1);
            (*normal).push_back(p2);
            (*normal).push_back(p2);
            (*normal).push_back(p1);
            (*normal).push_back(p2);
            (*normal).push_back(p1);

            res = (float)(stacks - i) / (float)stacks;
            resNext = (float)(stacks - (i + 1)) / (float)stacks;

            (*texture).push_back(0.75f + 0.25f * cos(theta) * res);
            (*texture).push_back(0.5f + 0.5f * sin(theta) * res);
            (*texture).push_back(0.75f + 0.25f * cos(thetaNext) * res);
            (*texture).push_back(0.5f + 0.5f * sin(thetaNext) * res);
            (*texture).push_back(0.75f + 0.25f * cos(thetaNext) * resNext);
            (*texture).push_back(0.5f + 0.5f * sin(thetaNext) * resNext);
            (*texture).push_back(0.75f + 0.25f * cos(theta) * res);
            (*texture).push_back(0.5f + 0.5f * sin(theta) * res);
            (*texture).push_back(0.75f + 0.25f * cos(thetaNext) * resNext);
            (*texture).push_back(0.5f + 0.5f * sin(thetaNext) * resNext);
            (*texture).push_back(0.75f + 0.25f * cos(theta) * resNext);
            (*texture).push_back(0.5f + 0.5f * sin(theta) * resNext);
        }
    }
    return vertices;
}

vector<Vertex> generate_torus(float inner_radius, float outer_radius, int slices, int stacks,
                              vector<Vertex> *normal, vector<float> *texture) {
    vector<Vertex> vertices;
    float theta = 0;
    float phi = 0;
    float phi_shift = (2 * M_PI) / stacks;
    float theta_shift = (2 * M_PI) / slices;

    float textureSide = float(1) / float(stacks);
    float textureRing = float(1) / float(slices);

    for (int i = 0; i < slices; i++) {
        double a0 = i * theta_shift;
        double a1 = a0 + theta_shift;

        float x0 = cos(a0);
        float y0 = sin(a0);
        float x1 = cos(a1);
        float y1 = sin(a1);

        for (int j = 0; j < stacks; j++) {
            vertices.push_back(Vertex(cos(theta) * (inner_radius + outer_radius * cos(phi)),
                                      sin(theta) * (inner_radius + outer_radius * cos(phi)),
                                      outer_radius * sin(phi)));
            vertices.push_back(
                Vertex((cos(theta + theta_shift) * (inner_radius + outer_radius * cos(phi))),
                       (sin(theta + theta_shift) * (inner_radius + outer_radius * cos(phi))),
                       (outer_radius * sin(phi))));
            vertices.push_back(Vertex(
                (cos(theta + theta_shift) * (inner_radius + outer_radius * cos(phi + phi_shift))),
                (sin(theta + theta_shift) * (inner_radius + outer_radius * cos(phi + phi_shift))),
                (outer_radius * sin(phi + phi_shift))));

            (*normal).push_back(
                Vertex(x0 * cos(j * phi_shift), y0 * cos(j * phi_shift), sin(j * phi_shift)));
            (*normal).push_back(
                Vertex(x1 * cos(j * phi_shift), y1 * cos(j * phi_shift), sin(j * phi_shift)));
            (*normal).push_back(Vertex(x0 * cos((j + 1) * phi_shift), y0 * cos((j + 1) * phi_shift),
                                       sin((j + 1) * phi_shift)));

            texture->push_back(i * textureRing);
            texture->push_back(j * textureSide);
            texture->push_back((i + 1) * textureRing);
            texture->push_back(j * textureSide);
            texture->push_back(i * textureRing);
            texture->push_back((j + 1) * textureSide);

            vertices.push_back(Vertex(
                (cos(theta + theta_shift) * (inner_radius + outer_radius * cos(phi + phi_shift))),
                (sin(theta + theta_shift) * (inner_radius + outer_radius * cos(phi + phi_shift))),
                (outer_radius * sin(phi + phi_shift))));
            vertices.push_back(
                Vertex((cos(theta) * (inner_radius + outer_radius * cos(phi + phi_shift))),
                       (sin(theta) * (inner_radius + outer_radius * cos(phi + phi_shift))),
                       (outer_radius * sin(phi + phi_shift))));
            vertices.push_back(Vertex((cos(theta) * (inner_radius + outer_radius * cos(phi))),
                                      (sin(theta) * (inner_radius + outer_radius * cos(phi))),
                                      (outer_radius * sin(phi))));
            (*normal).push_back(Vertex(x0 * cos((j + 1) * phi_shift), y0 * cos((j + 1) * phi_shift),
                                       sin((j + 1) * phi_shift)));
            (*normal).push_back(
                Vertex(x1 * cos(j * phi_shift), y1 * cos(j * phi_shift), sin(j * phi_shift)));
            (*normal).push_back(Vertex(x1 * cos((j + 1) * phi_shift), y1 * cos((j + 1) * phi_shift),
                                       sin((j + 1) * phi_shift)));

            texture->push_back(i * textureRing);
            texture->push_back((j + 1) * textureSide);
            texture->push_back((i + 1) * textureRing);
            ;
            texture->push_back(j * textureSide);
            texture->push_back((i + 1) * textureRing);
            texture->push_back((j + 1) * textureSide);

            phi = phi_shift * (j + 1);
        }
        theta = theta_shift * (i + 1);
    }
    return vertices;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Invalid number of arguments" << endl;
        cout << "Use -h option to see what parameters to pass" << endl;
        return 1;
    }

    if (!strcmp(argv[1], "-h")) {
        help();
        return 0;
    }

    vector<Vertex> vertices;
    vector<Vertex> normals;
    vector<float> textures;

    if (!strcmp(argv[1], "plane")) {
        if (argc != 4) {
            cout << "Invalid number of arguments to create plane" << endl;
            return 1;
        } else
            vertices = generate_plane(stof(argv[2]), &normals, &textures);
    } else if (!strcmp(argv[1], "box")) {
        if (argc < 6 || argc > 7) {
            cout << "Invalid number of arguments to create box" << endl;
            return 1;
        } else {
            vertices = argc <= 6 ? generate_box(stof(argv[2]), stof(argv[3]), stof(argv[4]), 1,
                                                &normals, &textures)
                                 : generate_box(stof(argv[2]), stof(argv[3]), stof(argv[4]),
                                                stof(argv[5]), &normals, &textures);
        }
    } else if (!strcmp(argv[1], "sphere")) {
        if (argc != 6) {
            cout << "Invalid number of arguments to create sphere" << endl;
            return 1;
        } else
            vertices =
                generate_sphere(stof(argv[2]), stoi(argv[3]), stoi(argv[4]), &normals, &textures);
    } else if (!strcmp(argv[1], "cone")) {
        if (argc != 7) {
            cout << "Invalid number of arguments to create cone" << endl;
            return 1;
        } else
            vertices = generate_cone(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi(argv[5]),
                                     &normals, &textures);
    } else if (!strcmp(argv[1], "torus")) {
        if (argc != 7) {
            cout << "Invalid number of arguments to create torus" << endl;
            return 1;
        } else
            vertices = generate_torus(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi(argv[5]),
                                      &normals, &textures);
    } else if (!strcmp(argv[1], "patch")) {
        if (argc != 5) {
            cout << "Invalid number of arguments to create patch" << endl;
            return 1;
        } else {
            Patch *p = new Patch(atoi(argv[2]), argv[3]);
            p->BezierModelGenerator(&vertices, &normals, &textures);
        }
    }

    write_file(argv[argc - 1], &vertices, &normals, &textures);
    return 0;
}
