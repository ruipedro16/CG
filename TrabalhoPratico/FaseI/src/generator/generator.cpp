#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "../common/vertex.hpp"

int write_file(const char *filename, std::vector<Vertex> *vertices) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open file" << std::endl;
        return 1;
    }
    for (std::vector<Vertex>::const_iterator it = vertices->begin(); it != vertices->end(); ++it)
        file << (*it).to_string() << std::endl;

    file.close();
    return 0;
}

void help() {
    std::cout << "Usage: ./generator <shape> [options] <file>" << std::endl;
    std::cout << "\nShapes & Options:" << std::endl;
    std::cout << "\tplane <size>" << std::endl;
    std::cout << "\tbox <width> <height> <length>" << std::endl;
    std::cout << "\tbox <width> <height> <length> <divisions>" << std::endl;
    std::cout << "\tsphere <radius> <slices> <stacks>" << std::endl;
    std::cout << "\tcone <radius> <height> <slices> <stacks>" << std::endl;
}

std::vector<Vertex> generate_plane(float side_length) {
    std::vector<Vertex> coords;
    float max_len = side_length / 2;

    coords.push_back(Vertex(max_len, 0.0f, max_len));
    coords.push_back(Vertex(-max_len, 0.0f, -max_len));
    coords.push_back(Vertex(-max_len, 0.0f, max_len));

    coords.push_back(Vertex(-max_len, 0.0f, -max_len));
    coords.push_back(Vertex(max_len, 0.0f, max_len));
    coords.push_back(Vertex(max_len, 0.0f, -max_len));

    return coords;
}

std::vector<Vertex> generate_box(float width, float height, float length, int divisions) {
    std::vector<Vertex> coords;
    float dx = width / divisions;
    float dy = height / divisions;
    float dz = length / divisions;

    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            // Frente
            coords.push_back(Vertex(dx * i - width / 2, dy * j - height / 2 + dy, length / 2));
            coords.push_back(Vertex(dx * i - width / 2 + dx, dy * j - height / 2, length / 2));
            coords.push_back(Vertex(dx * i - width / 2 + dx, dy * j - height / 2 + dy, length / 2));

            coords.push_back(Vertex(dx * i - width / 2, dy * j - height / 2 + dy, length / 2));
            coords.push_back(Vertex(dx * i - width / 2, dy * j - height / 2, length / 2));
            coords.push_back(Vertex(dx * i - width / 2 + dx, dy * j - height / 2, length / 2));

            // Trás
            coords.push_back(Vertex(dx * i - width / 2, dy * j - height / 2, -length / 2));
            coords.push_back(Vertex(dx * i - width / 2, dy * j - height / 2 + dy, -length / 2));
            coords.push_back(Vertex(dx * i - width / 2 + dx, dy * j - height / 2, -length / 2));

            coords.push_back(Vertex(dx * i - width / 2 + dx, dy * j - height / 2, -length / 2));
            coords.push_back(Vertex(dx * i - width / 2, dy * j - height / 2 + dy, -length / 2));
            coords.push_back(
                Vertex(dx * i - width / 2 + dx, dy * j - height / 2 + dy, -length / 2));

            // Face lateral
            coords.push_back(Vertex(width / 2, dy * i - height / 2, dz * j - length / 2));
            coords.push_back(Vertex(width / 2, dy * i - height / 2 + dy, dz * j - length / 2));
            coords.push_back(Vertex(width / 2, dy * i - height / 2, dz * j - length / 2 + dz));

            coords.push_back(Vertex(width / 2, dy * i - height / 2, dz * j - length / 2 + dz));
            coords.push_back(Vertex(width / 2, dy * i - height / 2 + dy, dz * j - length / 2));
            coords.push_back(Vertex(width / 2, dy * i - height / 2 + dy, dz * j - length / 2 + dz));

            // Face Lateral
            coords.push_back(Vertex(-width / 2, dy * i - height / 2, dz * j - length / 2 + dz));
            coords.push_back(Vertex(-width / 2, dy * i - height / 2 + dy, dz * j - length / 2));
            coords.push_back(Vertex(-width / 2, dy * i - height / 2, dz * j - length / 2));

            coords.push_back(
                Vertex(-width / 2, dy * i - height / 2 + dy, dz * j - length / 2 + dz));
            coords.push_back(Vertex(-width / 2, dy * i - height / 2 + dy, dz * j - length / 2));
            coords.push_back(Vertex(-width / 2, dy * i - height / 2, dz * j - length / 2 + dz));

            // Topo
            coords.push_back(Vertex(dx * j - width / 2, height / 2, dz * i - length / 2));
            coords.push_back(Vertex(dx * j - width / 2, height / 2, dz * i - length / 2 + dz));
            coords.push_back(Vertex(dx * j - width / 2 + dx, height / 2, dz * i - length / 2));

            coords.push_back(Vertex(dx * j - width / 2, height / 2, dz * i - length / 2 + dz));
            coords.push_back(Vertex(dx * j - width / 2 + dx, height / 2, dz * i - length / 2 + dz));
            coords.push_back(Vertex(dx * j - width / 2 + dx, height / 2, dz * i - length / 2));

            // Base
            coords.push_back(Vertex(dx * j - width / 2, -height / 2, dz * i - length / 2));
            coords.push_back(Vertex(dx * j - width / 2 + dx, -height / 2, dz * i - length / 2));
            coords.push_back(Vertex(dx * j - width / 2, -height / 2, dz * i - length / 2 + dz));

            coords.push_back(Vertex(dx * j - width / 2, -height / 2, dz * i - length / 2 + dz));
            coords.push_back(Vertex(dx * j - width / 2 + dx, -height / 2, dz * i - length / 2));
            coords.push_back(
                Vertex(dx * j - width / 2 + dx, -height / 2, dz * i - length / 2 + dz));
        }
    }
    return coords;
}

std::vector<Vertex> generate_sphere(float radius, int slices, int stacks) {
    std::vector<Vertex> coords;
    float alpha = 2 * M_PI / slices;
    float beta = M_PI / stacks;
    for (int j = 0; j < stacks; j++) {
        for (int i = 0; i < slices; i++) {
            coords.push_back(Vertex(cos(alpha * i) * cos(beta * j) * radius, sin(beta * j) * radius,
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
            coords.push_back(Vertex(cos(alpha * (i + 1)) * cos(beta * (j + 1)) * radius,
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
            coords.push_back(Vertex(cos(-alpha * (i + 1)) * cos(-beta * (j + 1)) * radius,
                                    sin(-beta * (j + 1)) * radius,
                                    sin(-alpha * (i + 1)) * cos(-beta * (j + 1)) * radius));
        }
    }
    return coords;
}

std::vector<Vertex> generate_cone(float radius, float height, int slices, int stacks) {
    std::vector<Vertex> coords;
    double alpha = (2 * M_PI) / slices;
    double h = height / stacks;
    double d_radius = radius / stacks;

    // Base
    for (int i = 0; i < slices; i++) {
        coords.push_back(Vertex(0.0, 0.0, 0.0));
        coords.push_back(Vertex(cos(alpha * i) * radius, 0.0, sin(alpha * i) * radius));
        coords.push_back(Vertex(cos(alpha * (i + 1)) * radius, 0.0, sin(alpha * (i + 1)) * radius));
    }

    // Superficie
    for (int j = 0; j < stacks; j++, radius -= d_radius) {
        for (int i = 0; i < slices; i++) {
            coords.push_back(Vertex(cos(alpha * i) * radius, h * j, sin(alpha * i) * radius));
            coords.push_back(Vertex(cos(alpha * i) * (radius - d_radius), h * (j + 1),
                                    sin(alpha * i) * (radius - d_radius)));
            coords.push_back(
                Vertex(cos(alpha * (i + 1)) * radius, h * j, sin(alpha * (i + 1)) * radius));

            coords.push_back(
                Vertex(cos(alpha * (i + 1)) * radius, h * j, sin(alpha * (i + 1)) * radius));
            coords.push_back(Vertex(cos(alpha * i) * (radius - d_radius), h * (j + 1),
                                    sin(alpha * i) * (radius - d_radius)));
            coords.push_back(Vertex(cos(alpha * (i + 1)) * (radius - d_radius), h * (j + 1),
                                    sin(alpha * (i + 1)) * (radius - d_radius)));
        }
    }

    return coords;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Invalid number of arguments" << std::endl;
        std::cout << "Use -h option to see what parameters to pass" << std::endl;
        return 1;
    }

    if (!strcmp(argv[1], "-h")) {
        help();
        return 0;
    }

    std::vector<Vertex> vertices;

    if (!strcmp(argv[1], "plane")) {
        if (argc != 4) {
            std::cout << "Invalid number of arguments to create plane" << std::endl;
            return 1;
        } else
            vertices = generate_plane(std::atof(argv[2]));
    } else if (!strcmp(argv[1], "box")) {
        if (argc < 6 || argc > 7) {
            std::cout << "Invalid number of arguments to create box" << std::endl;
            return 1;
        } else {
            vertices = argc <= 6 ? generate_box(std::atof(argv[2]), std::atof(argv[3]),
                                                std::atof(argv[4]), 1)
                                 : generate_box(std::atof(argv[2]), std::atof(argv[3]),
                                                std::atof(argv[4]), std::atof(argv[5]));
        }
    } else if (!strcmp(argv[1], "sphere")) {
        if (argc != 6) {
            std::cout << "Inavalid number of arguments to create sphere" << std::endl;
            return 1;
        } else
            vertices = generate_sphere(std::atof(argv[2]), std::atoi(argv[3]), std::atoi(argv[4]));
    } else if (!strcmp(argv[1], "cone")) {
        if (argc != 7) {
            std::cout << "Inavalid number of arguments to create cone" << std::endl;
            return 1;
        } else
            vertices = generate_cone(std::atof(argv[2]), std::atof(argv[3]), std::atoi(argv[4]),
                                     std::atoi(argv[5]));
    }

    write_file(argv[argc - 1], &vertices);
    return 0;
}
