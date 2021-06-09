#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "../common/vertex.hpp"
#include "patch.hpp"

using namespace std;

int write_file(const char *filepath, vector<Vertex> *vertices) {
  ofstream file(filepath);
  if (!file.is_open()) {
    cout << "Could not open file " << filepath << endl;
    return 1;
  }
  for (vector<Vertex>::const_iterator it = vertices->begin();
       it != vertices->end(); ++it)
    file << (*it).to_string() << endl;
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
  cout << "\ttorus <inner radius> <outer radius> <slices> <stacks>" << endl;
  cout << "\tpatch <tesselation> <patch file>" << endl;
}

vector<Vertex> generate_plane(float side_length) {
  vector<Vertex> coords;
  float max_len = side_length / 2;

  coords.push_back(Vertex(max_len, 0.0f, max_len));
  coords.push_back(Vertex(-max_len, 0.0f, -max_len));
  coords.push_back(Vertex(-max_len, 0.0f, max_len));

  coords.push_back(Vertex(-max_len, 0.0f, -max_len));
  coords.push_back(Vertex(max_len, 0.0f, max_len));
  coords.push_back(Vertex(max_len, 0.0f, -max_len));

  return coords;
}

vector<Vertex> generate_box(float width, float height, float length,
                            int divisions) {
  vector<Vertex> coords;
  float dx = width / divisions;
  float dy = height / divisions;
  float dz = length / divisions;

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      // Frente
      coords.push_back(
          Vertex(dx * i - width / 2, dy * j - height / 2 + dy, length / 2));
      coords.push_back(
          Vertex(dx * i - width / 2 + dx, dy * j - height / 2, length / 2));
      coords.push_back(Vertex(dx * i - width / 2 + dx, dy * j - height / 2 + dy,
                              length / 2));

      coords.push_back(
          Vertex(dx * i - width / 2, dy * j - height / 2 + dy, length / 2));
      coords.push_back(
          Vertex(dx * i - width / 2, dy * j - height / 2, length / 2));
      coords.push_back(
          Vertex(dx * i - width / 2 + dx, dy * j - height / 2, length / 2));

      // Trás
      coords.push_back(
          Vertex(dx * i - width / 2, dy * j - height / 2, -length / 2));
      coords.push_back(
          Vertex(dx * i - width / 2, dy * j - height / 2 + dy, -length / 2));
      coords.push_back(
          Vertex(dx * i - width / 2 + dx, dy * j - height / 2, -length / 2));

      coords.push_back(
          Vertex(dx * i - width / 2 + dx, dy * j - height / 2, -length / 2));
      coords.push_back(
          Vertex(dx * i - width / 2, dy * j - height / 2 + dy, -length / 2));
      coords.push_back(Vertex(dx * i - width / 2 + dx, dy * j - height / 2 + dy,
                              -length / 2));

      // Face lateral
      coords.push_back(
          Vertex(width / 2, dy * i - height / 2, dz * j - length / 2));
      coords.push_back(
          Vertex(width / 2, dy * i - height / 2 + dy, dz * j - length / 2));
      coords.push_back(
          Vertex(width / 2, dy * i - height / 2, dz * j - length / 2 + dz));

      coords.push_back(
          Vertex(width / 2, dy * i - height / 2, dz * j - length / 2 + dz));
      coords.push_back(
          Vertex(width / 2, dy * i - height / 2 + dy, dz * j - length / 2));
      coords.push_back(Vertex(width / 2, dy * i - height / 2 + dy,
                              dz * j - length / 2 + dz));

      // Face Lateral
      coords.push_back(
          Vertex(-width / 2, dy * i - height / 2, dz * j - length / 2 + dz));
      coords.push_back(
          Vertex(-width / 2, dy * i - height / 2 + dy, dz * j - length / 2));
      coords.push_back(
          Vertex(-width / 2, dy * i - height / 2, dz * j - length / 2));

      coords.push_back(Vertex(-width / 2, dy * i - height / 2 + dy,
                              dz * j - length / 2 + dz));
      coords.push_back(
          Vertex(-width / 2, dy * i - height / 2 + dy, dz * j - length / 2));
      coords.push_back(
          Vertex(-width / 2, dy * i - height / 2, dz * j - length / 2 + dz));

      // Topo
      coords.push_back(
          Vertex(dx * j - width / 2, height / 2, dz * i - length / 2));
      coords.push_back(
          Vertex(dx * j - width / 2, height / 2, dz * i - length / 2 + dz));
      coords.push_back(
          Vertex(dx * j - width / 2 + dx, height / 2, dz * i - length / 2));

      coords.push_back(
          Vertex(dx * j - width / 2, height / 2, dz * i - length / 2 + dz));
      coords.push_back(Vertex(dx * j - width / 2 + dx, height / 2,
                              dz * i - length / 2 + dz));
      coords.push_back(
          Vertex(dx * j - width / 2 + dx, height / 2, dz * i - length / 2));

      // Base
      coords.push_back(
          Vertex(dx * j - width / 2, -height / 2, dz * i - length / 2));
      coords.push_back(
          Vertex(dx * j - width / 2 + dx, -height / 2, dz * i - length / 2));
      coords.push_back(
          Vertex(dx * j - width / 2, -height / 2, dz * i - length / 2 + dz));

      coords.push_back(
          Vertex(dx * j - width / 2, -height / 2, dz * i - length / 2 + dz));
      coords.push_back(
          Vertex(dx * j - width / 2 + dx, -height / 2, dz * i - length / 2));
      coords.push_back(Vertex(dx * j - width / 2 + dx, -height / 2,
                              dz * i - length / 2 + dz));
    }
  }
  return coords;
}

vector<Vertex> generate_sphere(float radius, int slices, int stacks) {
  vector<Vertex> coords;
  float alpha = 2 * M_PI / slices;
  float beta = M_PI / stacks;
  for (int j = 0; j < stacks; j++) {
    for (int i = 0; i < slices; i++) {
      coords.push_back(Vertex(cos(alpha * i) * cos(beta * j) * radius,
                              sin(beta * j) * radius,
                              sin(alpha * i) * cos(beta * j) * radius));
      coords.push_back(Vertex(cos(alpha * i) * cos(beta * (j + 1)) * radius,
                              sin(beta * (j + 1)) * radius,
                              sin(alpha * i) * cos(beta * (j + 1)) * radius));
      coords.push_back(Vertex(cos(alpha * (i + 1)) * cos(beta * j) * radius,
                              sin(beta * j) * radius,
                              sin(alpha * (i + 1)) * cos(beta * j) * radius));

      coords.push_back(Vertex(cos(alpha * (i + 1)) * cos(beta * j) * radius,
                              sin(beta * j) * radius,
                              sin(alpha * (i + 1)) * cos(beta * j) * radius));
      coords.push_back(Vertex(cos(alpha * i) * cos(beta * (j + 1)) * radius,
                              sin(beta * (j + 1)) * radius,
                              sin(alpha * i) * cos(beta * (j + 1)) * radius));
      coords.push_back(
          Vertex(cos(alpha * (i + 1)) * cos(beta * (j + 1)) * radius,
                 sin(beta * (j + 1)) * radius,
                 sin(alpha * (i + 1)) * cos(beta * (j + 1)) * radius));

      coords.push_back(Vertex(cos(-alpha * i) * cos(-beta * j) * radius,
                              sin(-beta * j) * radius,
                              sin(-alpha * i) * cos(-beta * j) * radius));
      coords.push_back(Vertex(cos(-alpha * i) * cos(-beta * (j + 1)) * radius,
                              sin(-beta * (j + 1)) * radius,
                              sin(-alpha * i) * cos(-beta * (j + 1)) * radius));
      coords.push_back(Vertex(cos(-alpha * (i + 1)) * cos(-beta * j) * radius,
                              sin(-beta * j) * radius,
                              sin(-alpha * (i + 1)) * cos(-beta * j) * radius));

      coords.push_back(Vertex(cos(-alpha * (i + 1)) * cos(-beta * j) * radius,
                              sin(-beta * j) * radius,
                              sin(-alpha * (i + 1)) * cos(-beta * j) * radius));
      coords.push_back(Vertex(cos(-alpha * i) * cos(-beta * (j + 1)) * radius,
                              sin(-beta * (j + 1)) * radius,
                              sin(-alpha * i) * cos(-beta * (j + 1)) * radius));
      coords.push_back(
          Vertex(cos(-alpha * (i + 1)) * cos(-beta * (j + 1)) * radius,
                 sin(-beta * (j + 1)) * radius,
                 sin(-alpha * (i + 1)) * cos(-beta * (j + 1)) * radius));
    }
  }
  return coords;
}

vector<Vertex> generate_cone(float radius, float height, int slices,
                             int stacks) {
  vector<Vertex> coords;
  double alpha = 2 * M_PI / slices;
  double h = height / stacks;
  double d_radius = radius / stacks;

  // Base
  for (int i = 0; i < slices; i++) {
    coords.push_back(Vertex(0.0, 0.0, 0.0));
    coords.push_back(
        Vertex(cos(alpha * i) * radius, 0.0, sin(alpha * i) * radius));
    coords.push_back(Vertex(cos(alpha * (i + 1)) * radius, 0.0,
                            sin(alpha * (i + 1)) * radius));
  }

  // Superficie
  for (int j = 0; j < stacks; j++, radius -= d_radius) {
    for (int i = 0; i < slices; i++) {
      coords.push_back(
          Vertex(cos(alpha * i) * radius, h * j, sin(alpha * i) * radius));
      coords.push_back(Vertex(cos(alpha * i) * (radius - d_radius), h * (j + 1),
                              sin(alpha * i) * (radius - d_radius)));
      coords.push_back(Vertex(cos(alpha * (i + 1)) * radius, h * j,
                              sin(alpha * (i + 1)) * radius));

      coords.push_back(Vertex(cos(alpha * (i + 1)) * radius, h * j,
                              sin(alpha * (i + 1)) * radius));
      coords.push_back(Vertex(cos(alpha * i) * (radius - d_radius), h * (j + 1),
                              sin(alpha * i) * (radius - d_radius)));
      coords.push_back(Vertex(cos(alpha * (i + 1)) * (radius - d_radius),
                              h * (j + 1),
                              sin(alpha * (i + 1)) * (radius - d_radius)));
    }
  }

  return coords;
}

vector<Vertex> generate_torus(float inner_radius, float outer_radius,
                              int slices, int stacks) {
  vector<Vertex> coords;

  float theta = 0;
  float phi = 0;
  float theta_shift = 2 * M_PI / slices;
  float phi_shift = 2 * M_PI / stacks;

  for (int i = 0; i < slices; i++, theta += theta_shift) {
    for (int j = 0; j < stacks; j++, phi += phi_shift) {
      coords.push_back(
          Vertex(cos(theta) * (outer_radius + inner_radius * cos(phi)),
                 sin(theta) * (outer_radius + inner_radius * cos(phi)),
                 inner_radius * sin(phi)));
      coords.push_back(Vertex(
          cos(theta + theta_shift) * (outer_radius + inner_radius * cos(phi)),
          sin(theta + theta_shift) * (outer_radius + inner_radius * cos(phi)),
          inner_radius * sin(phi)));
      coords.push_back(
          Vertex(cos(theta + theta_shift) *
                     (outer_radius + inner_radius * cos(phi + phi_shift)),
                 sin(theta + theta_shift) *
                     (outer_radius + inner_radius * cos(phi + phi_shift)),
                 inner_radius * sin(phi + phi_shift)));
      coords.push_back(
          Vertex(cos(theta + theta_shift) *
                     (outer_radius + inner_radius * cos(phi + phi_shift)),
                 sin(theta + theta_shift) *
                     (outer_radius + inner_radius * cos(phi + phi_shift)),
                 inner_radius * sin(phi + phi_shift)));
      coords.push_back(Vertex(
          cos(theta) * (outer_radius + inner_radius * cos(phi + phi_shift)),
          sin(theta) * (outer_radius + inner_radius * cos(phi + phi_shift)),
          inner_radius * sin(phi + phi_shift)));
      coords.push_back(
          Vertex(cos(theta) * (outer_radius + inner_radius * cos(phi)),
                 sin(theta) * (outer_radius + inner_radius * cos(phi)),
                 inner_radius * sin(phi)));
    }
  }
  return coords;
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

  if (!strcmp(argv[1], "plane")) {
    if (argc != 4) {
      cout << "Invalid number of arguments to create plane" << endl;
      return 1;
    } else
      vertices = generate_plane(stof(argv[2]));
  } else if (!strcmp(argv[1], "box")) {
    if (argc < 6 || argc > 7) {
      cout << "Invalid number of arguments to create box" << endl;
      return 1;
    } else {
      vertices = argc <= 6 ? generate_box(stof(argv[2]), stof(argv[3]),
                                          stof(argv[4]), 1)
                           : generate_box(stof(argv[2]), stof(argv[3]),
                                          stof(argv[4]), stof(argv[5]));
    }
  } else if (!strcmp(argv[1], "sphere")) {
    if (argc != 6) {
      cout << "Invalid number of arguments to create sphere" << endl;
      return 1;
    } else
      vertices = generate_sphere(stof(argv[2]), stoi(argv[3]), stoi(argv[4]));
  } else if (!strcmp(argv[1], "cone")) {
    if (argc != 7) {
      cout << "Invalid number of arguments to create cone" << endl;
      return 1;
    } else
      vertices = generate_cone(stof(argv[2]), stof(argv[3]), stoi(argv[4]),
                               stoi(argv[5]));
  } else if (!strcmp(argv[1], "torus")) {
    if (argc != 7) {
      cout << "Invalid number of arguments to create torus" << endl;
      return 1;
    } else
      vertices = generate_torus(stof(argv[2]), stof(argv[3]), stoi(argv[4]),
                                stoi(argv[5]));
  } else if (!strcmp(argv[1], "patch")) {
    if (argc != 5) {
      cout << "Invalid number of arguments to create patch" << endl;
      return 1;
    } else {
      Patch *p = new Patch(atoi(argv[2]), argv[3]);
      vertices = p->BezierModelGenerator();
    }
  }

  write_file(argv[argc - 1], &vertices);
  return 0;
}
