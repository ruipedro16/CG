#include "patch.hpp"

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
    for (int j = 0; j < 4; ++j)
      res[i] += v[j] * m[i * 4 + j];
  }
}

Vertex *Patch::getPoint(float ta, float tb, float (*coordX)[4],
                        float (*coordY)[4], float (*coordZ)[4]) {
  float x = 0.0f, y = 0.0f, z = 0.0f;
  float m[4][4] = {{-1.0f, 3.0f, -3.0f, 1.0f},
                   {3.0f, -6.0f, 3.0f, 0.0f},
                   {-3.0f, 3.0f, 0.0f, 0.0f},
                   {1.0f, 0.0f, 0.0f, 0.0f}};

  float a[4] = {ta * ta * ta, ta * ta, ta, 1.0f};
  float b[4] = {tb * tb * tb, tb * tb, tb, 1.0f};

  float am[4];
  multMatrixVector(*m, a, am);

  float bm[4];
  multMatrixVector(*m, b, bm);

  float amCoordenadaX[4], amCoordenadaY[4], amCoordenadaZ[4];
  multMatrixVector(*coordX, am, amCoordenadaX);
  multMatrixVector(*coordY, am, amCoordenadaY);
  multMatrixVector(*coordZ, am, amCoordenadaZ);

  for (int i = 0; i < 4; i++) {
    x += amCoordenadaX[i] * bm[i];
    y += amCoordenadaY[i] * bm[i];
    z += amCoordenadaZ[i] * bm[i];
  }

  Vertex *p = new Vertex(x, y, z);
  return p;
}

vector<Vertex> Patch::getPatchPoints(int patch) {
  vector<Vertex> points;
  vector<int> indexesControlPoints = patches.at(patch);

  float x_coordinates[4][4], y_coordinates[4][4], z_coordinates[4][4];
  float u, v, uu, vv;
  float t = 1.0f / (float)tesselation;
  int pos = 0;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++, pos++) {
      Vertex controlPoint = controlPoints[indexesControlPoints[pos]];
      x_coordinates[i][j] = controlPoint.getX();
      y_coordinates[i][j] = controlPoint.getY();
      z_coordinates[i][j] = controlPoint.getZ();
    }
  }

  for (int i = 0; i < tesselation; i++) {
    for (int j = 0; j < tesselation; j++) {
      u = (float)i * t;
      v = (float)j * t;
      uu = (float)(i + 1) * t;
      vv = (float)(j + 1) * t;
      Vertex *p0, *p1, *p2, *p3;
      p0 = getPoint(u, v, x_coordinates, y_coordinates, z_coordinates);
      p1 = getPoint(u, vv, x_coordinates, y_coordinates, z_coordinates);
      p2 = getPoint(uu, v, x_coordinates, y_coordinates, z_coordinates);
      p3 = getPoint(uu, vv, x_coordinates, y_coordinates, z_coordinates);

      points.push_back(*p0);
      points.push_back(*p2);
      points.push_back(*p1);
      points.push_back(*p1);
      points.push_back(*p2);
      points.push_back(*p3);
    }
  }
  return points;
}

vector<Vertex> Patch::BezierModelGenerator() {
  vector<Vertex> res;
  for (int i = 0; i < nPatches; i++) {
    vector<Vertex> aux = getPatchPoints(i);
    res.insert(res.end(), aux.begin(), aux.end());
  }
  return res;
}
