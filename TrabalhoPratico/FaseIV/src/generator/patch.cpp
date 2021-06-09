#include "patch.hpp"

#include <math.h>

using namespace std;

Patch::Patch(vector<Vertex> p) { controlPoints = p; }

Patch::Patch(int tess, const char *file) {
    tesselation = tess;
    parsePatchFile(file);
}

void Patch::parsePatchFile(const char *filename) {
    int i;
    string line, x, y, z;

    ifstream file(filename);
    if (file.is_open()) {
        getline(file, line);
        nPatches = stoi(line);

        for (i = 0; i < nPatches; i++) {
            vector<int> patchIndex;

            if (getline(file, line)) {
                char *str = strdup(line.c_str());
                char *token = strtok(str, " ,");

                while (token != NULL) {
                    patchIndex.push_back(atoi(token));
                    token = strtok(NULL, " ,");
                }

                patches[i] = patchIndex;
                free(str);
            } else {
                cout << "Unable to get all patchIndex" << endl;
            }
        }

        getline(file, line);
        nPoints = stoi(line);

        for (i = 0; i < nPoints; i++) {
            if (getline(file, line)) {
                char *str = strdup(line.c_str());
                char *token = strtok(str, " ,");
                float x = atof(token);
                token = strtok(NULL, " ,");
                float y = atof(token);
                token = strtok(NULL, " ,");
                float z = atof(token);
                Vertex *p = new Vertex(x, y, z);
                controlPoints.push_back(*p);
                free(str);
            } else {
                cout << "Unable to get all control points" << endl;
            }
        }

        file.close();
    } else {
        cout << "Error opening file " << filename << endl;
    }
}

void Patch::multMatrixVector(float *m, float *v, float *res) {
    for (int i = 0; i < 4; ++i) {
        res[i] = 0;
        for (int j = 0; j < 4; ++j) res[i] += v[j] * m[i * 4 + j];
    }
}

Vertex *Patch::getPoint(float ta, float tb, float (*coordX)[4], float (*coordY)[4],
                        float (*coordZ)[4]) {
    float x = 0.0f, y = 0.0f, z = 0.0f;

    float a[4] = {ta * ta * ta, ta * ta, ta, 1.0f};
    float b[4] = {tb * tb * tb, tb * tb, tb, 1.0f};

    float am[4];
    multMatrixVector(*m, a, am);

    float bm[4];
    multMatrixVector(*m, b, bm);

    float amCoordX[4], amCoordY[4], amCoordZ[4];
    multMatrixVector(*coordX, am, amCoordX);
    multMatrixVector(*coordY, am, amCoordY);
    multMatrixVector(*coordZ, am, amCoordZ);

    for (int i = 0; i < 4; i++) {
        x += amCoordX[i] * bm[i];
        y += amCoordY[i] * bm[i];
        z += amCoordZ[i] * bm[i];
    }

    Vertex *p = new Vertex(x, y, z);
    return p;
}
void Patch::getPatchPoints(int patch, std::vector<Vertex> *points, std::vector<float> *texture,
                           std::vector<Vertex> *normal) {
    vector<int> indexesControlPoints = patches.at(patch);

    float x_coordinates[4][4], y_coordinates[4][4], z_coordinates[4][4];
    float u, v, uu, vv;
    float t = 1.0f / (float)tesselation;
    int pos = 0;
    float *tangenteU, *tangenteV, res[3];
    Vertex *p0, *p1, *p2, *p3;
    Vertex *n0, *n1, *n2, *n3;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            Vertex controlPoint = controlPoints[indexesControlPoints[pos]];
            x_coordinates[i][j] = controlPoint.getX();
            y_coordinates[i][j] = controlPoint.getY();
            z_coordinates[i][j] = controlPoint.getZ();
            pos++;
        }
    }

    for (int i = 0; i < tesselation; i++) {
        for (int j = 0; j < tesselation; j++) {
            u = (float)i * t;
            v = (float)j * t;
            uu = (float)(i + 1) * t;
            vv = (float)(j + 1) * t;

            p0 = getPoint(u, v, x_coordinates, y_coordinates, z_coordinates);
            tangenteU = getTangent(u, v, x_coordinates, y_coordinates, z_coordinates, 0);
            tangenteV = getTangent(u, v, x_coordinates, y_coordinates, z_coordinates, 1);
            cross(tangenteU, tangenteV, res);
            normalize(res);
            n0 = new Vertex(res[0], res[1], res[2]);

            p1 = getPoint(u, vv, x_coordinates, y_coordinates, z_coordinates);
            tangenteU = getTangent(u, v, x_coordinates, y_coordinates, z_coordinates, 0);
            tangenteV = getTangent(u, v, x_coordinates, y_coordinates, z_coordinates, 1);
            cross(tangenteU, tangenteV, res);
            normalize(res);
            n1 = new Vertex(res[0], res[1], res[2]);

            p2 = getPoint(uu, v, x_coordinates, y_coordinates, z_coordinates);
            tangenteU = getTangent(u, v, x_coordinates, y_coordinates, z_coordinates, 0);
            tangenteV = getTangent(u, v, x_coordinates, y_coordinates, z_coordinates, 1);
            cross(tangenteU, tangenteV, res);
            normalize(res);
            n2 = new Vertex(res[0], res[1], res[2]);

            p3 = getPoint(uu, vv, x_coordinates, y_coordinates, z_coordinates);
            tangenteU = getTangent(u, v, x_coordinates, y_coordinates, z_coordinates, 0);
            tangenteV = getTangent(u, v, x_coordinates, y_coordinates, z_coordinates, 1);
            cross(tangenteU, tangenteV, res);
            normalize(res);
            n3 = new Vertex(res[0], res[1], res[2]);

            points->push_back(*p0);
            points->push_back(*p2);
            points->push_back(*p1);
            points->push_back(*p1);
            points->push_back(*p2);
            points->push_back(*p3);

            normal->push_back(*n0);
            normal->push_back(*n2);
            normal->push_back(*n1);
            normal->push_back(*n1);
            normal->push_back(*n2);
            normal->push_back(*n3);

            texture->push_back(1 - u);
            texture->push_back(1 - v);
            texture->push_back(1 - uu);
            texture->push_back(1 - v);
            texture->push_back(1 - u);
            texture->push_back(1 - vv);
            texture->push_back(1 - u);
            texture->push_back(1 - vv);
            texture->push_back(1 - uu);
            texture->push_back(1 - v);
            texture->push_back(1 - uu);
            texture->push_back(1 - vv);
        }
    }
}
float *Patch ::getTangent(float tu, float tv, float mX[4][4], float mY[4][4], float mZ[4][4],
                          int type) {
    float u[4], v[4];

    if (!type) {
        u[0] = 3.0f * tu * tu;
        u[1] = 2.0f * tu;
        u[2] = 1.0f;
        u[3] = 0.0f;

        v[0] = tv * tv * tv;
        v[1] = tv * tv;
        v[2] = tv;
        v[3] = 1.0f;
    } else {
        u[0] = tu * tu * tu;
        u[1] = tu * tu;
        u[2] = tu;
        u[3] = 1.0f;

        v[0] = 3.0f * tv * tv;
        v[1] = 2.0f * tv;
        v[2] = 1.0f;
        v[3] = 0.0f;
    }

    float uM[4];
    multMatrixVector(*m, u, uM);

    float Mv[4];
    multMatrixVector(*m, v, Mv);

    float matX[4], matY[4], matZ[4];
    multMatrixVector(*mX, uM, matX);
    multMatrixVector(*mY, uM, matY);
    multMatrixVector(*mZ, uM, matZ);

    float *tang = (float *)calloc(3, sizeof(float));
    for (int i = 0; i < 4; i++) {
        tang[0] += matX[i] * Mv[i];
        tang[1] += matY[i] * Mv[i];
        tang[2] += matZ[i] * Mv[i];
    }
    return tang;
}

void Patch::normalize(float *a) {
    float n = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0] / n;
    a[1] = a[1] / n;
    a[2] = a[2] / n;
}

void Patch::cross(float *a, float *b, float *res) {
    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];
}

void Patch::BezierModelGenerator(vector<Vertex> *vert, vector<Vertex> *normal,
                                 vector<float> *texture) {
    for (int i = 0; i < nPatches; i++) getPatchPoints(i, vert, texture, normal);
}
