#include <GL/gl.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "../common/vertex.hpp"
#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

GLfloat cam_x = 9.0f, cam_y = 5.0f, cam_z = 9.0f;

GLfloat radius = sqrt(pow(cam_x, 2) + pow(cam_y, 2) + pow(cam_z, 2));
GLfloat alpha = atan(cam_y / cam_x);
GLfloat beta = acos(cam_z / (radius * cos(alpha)));

GLfloat scale_x = 1.0f, scale_y = 1.0f, scale_z = 1.0f;

GLfloat point_size = 1.0f;
GLenum mode = GL_LINE;

bool axis = true, fullscreen = false;

GLfloat x_axis_len = 0.0f, y_axis_len = 0.0f, z_axis_len = 0.0f;

std::string models_dir = "../models/";
std::vector<Vertex> vertices;

void spherical2cartesian() {
    cam_x = radius * sin(alpha) * cos(beta);
    cam_y = radius * sin(beta);
    cam_z = radius * cos(alpha) * cos(beta);
}

void help() {
    std::cout << "Usage: ./engine [-h] <XML file>" << std::endl;
    std::cout << "\nList of commands:" << std::endl;
    std::cout << "\tF1 : Increase point size" << std::endl;
    std::cout << "\tF2 : Decrease point size" << std::endl;
    std::cout << "\tF3 : Enable/Disable axis" << std::endl;
    std::cout << "\tF4 : Enable/Disable fullscreen" << std::endl;
    std::cout << "\t1 : Increase x axis length" << std::endl;
    std::cout << "\t2 : Decrease x axis length" << std::endl;
    std::cout << "\t3 : Increase y axis length" << std::endl;
    std::cout << "\t4 : Decrease y axis length" << std::endl;
    std::cout << "\t5 : Increase z axis length" << std::endl;
    std::cout << "\t6 : Decrease z axis length" << std::endl;
    std::cout << "\t+ : Zoom in" << std::endl;
    std::cout << "\t- : Zoom out" << std::endl;
    std::cout << "\tX : Zoom in along the X axis" << std::endl;
    std::cout << "\tx : Zoom out along the X axis" << std::endl;
    std::cout << "\tY : Zoom in along the Y axis" << std::endl;
    std::cout << "\ty : Zoom out along the Y axis" << std::endl;
    std::cout << "\tZ : Zoom in along the z axis" << std::endl;
    std::cout << "\tz : Zoom out along the z axis" << std::endl;
    std::cout << "\tw : Change the camera position upwards" << std::endl;
    std::cout << "\ta : Change the camera position to the left" << std::endl;
    std::cout << "\ts : Change the camera position downwards" << std::endl;
    std::cout << "\td : Change the camera position to the right" << std::endl;
    std::cout << "\tp: Change the figure format into points" << std::endl;
    std::cout << "\tl: Change the figure format into lines" << std::endl;
    std::cout << "\tf: Fill up the figure" << std::endl;
}

int parse_file(const char *path, std::vector<std::string> *models) {
    XMLDocument doc;
    if (doc.LoadFile(path) != XML_SUCCESS) {
        std::cout << "Could not open XML file " << path << std::endl;
        return 1;
    }
    XMLElement *scene = doc.FirstChildElement("scene");
    if (scene == nullptr) return 1;
    XMLElement *model = scene->FirstChildElement("model");
    for (; model != nullptr; model = model->NextSiblingElement())
        models->push_back(model->Attribute("file"));
    return 0;
}

int read_file(const char *path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Could not open file " << path << std::endl;
        return 1;
    }
    float x, y, z;
    while (file >> x >> y >> z) vertices.push_back(Vertex(x, y, z));
    file.close();
    return 0;
}

void draw_axis() {
    // Draw mode of Axis's Arrow Heads (FILLED)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Arrow Head of X Axis (RED)
    glRotatef(90.f, 0, 1, 0);  // Rotation of 90º Degrees, relative to the Y Axis... Now our Z axis,
                               // assumes the initial position of the X axis.
    glTranslatef(0, 0, 5 + x_axis_len);  // Translation of "5+x_axis_len", relative to the Z Axis
    glColor3f(1.0f, 0.0f, 0.0f);         // Color of our X Arrow Head
    glutSolidCone(0.1, 0.3, 5, 5);  // Arrow Head is drawn, as a solid cone. Base on Z=0, Height on
                                    // Z=0.3, Radius of 0.3 {5 slices and 5 stacks}.

    // This cone stays in the previously translated position. In this case "5+x_axis_len"
    glTranslatef(0, 0, -5 - x_axis_len);  // Translates to the initial position - origin
    glRotatef(-90.f, 0, 1, 0);            // Rotates to the initial position - origin

    // Arrow Head of Y Axis (GREEN)
    glRotatef(-90.f, 1, 0, 0);  // Rotation of -90º Degrees, relative to the X Axis... Now our Z
                                // axis, assumes the initial position of the Y axis.
    glTranslatef(0, 0, 5 + y_axis_len);  // Translation of "5+y_axis_len", relative to the Z Axis
    glColor3f(0.0f, 1.0f, 0.0f);         // Color of our Y Arrow Head
    glutSolidCone(0.1, 0.3, 5, 5);  // Arrow Head is drawn, as a solid cone. Base on Z=0, Height on
                                    // Z=0.3, Radius of 0.3 {5 slices and 5 stacks}.

    // This cone stays in the previously translated position. In this case "5+y_axis_len"
    glTranslatef(0, 0, -5 - y_axis_len);  // Translates to the initial position - origin
    glRotatef(90.f, 1, 0, 0);             // Rotates to the initial position - origin

    // Arrow Head of Z Axis (BLUE)
    glTranslatef(0, 0, 5 + z_axis_len);  // Translation of "5+z_axis_len", relative to the Z Axis
    glColor3f(0.0f, 0.0f, 1.0f);         // Color of our Z Arrow Head
    glutSolidCone(0.1, 0.3, 5, 5);  // Arrow Head is drawn, as a solid cone. Base on Z=0, Height on
                                    // Z=0.3, Radius of 0.3 {5 slices and 5 stacks}.

    // This cone stays in the previously translated position. In this case "5+z_axis_len"
    glTranslatef(0, 0, -5 - z_axis_len);  // Translates to the initial position - origin

    glBegin(GL_LINES);

    // Line of X Axis (RED) - From (0,0,0) to (5+x_axis_len,0,0)
    glColor3f(1.0, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(5 + x_axis_len, 0, 0);

    // Line of Y Axis (GREEN) - From (0,0,0) to (0,5+y_axis_len,0)
    glColor3f(0, 1.0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 5 + y_axis_len, 0);

    // Line of Z Axis (BLUE) - From (0,0,0) to (0,0,5+z_axis_len)
    glColor3f(0, 0, 1.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 5 + z_axis_len);

    glEnd();

    // Letter X coloring (RED) and positioning (5+x_axis_len+0.5,0,0)
    glColor3f(1.0, 0, 0);
    glRasterPos3f((5 + x_axis_len + 0.5), 0, 0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'X');

    // Letter Y coloring (GREEN) and positioning (0,5+y_axis_len+0.5,0)
    glColor3f(0, 1.0, 0);
    glRasterPos3f(0, (5 + y_axis_len + 0.5), 0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Y');

    // Letter Z coloring (BLUE) and positioning (0,0,5+z_axis_len+0.5)
    glColor3f(0, 0, 1.0);
    glRasterPos3f(0, 0, (5 + z_axis_len + 0.5));
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Z');
}

void changeSize(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you can't make a window with zero width).
    if (h == 0) h = 1;

    // Compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);

    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

    // Return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set camera
    glLoadIdentity();

    spherical2cartesian();
    gluLookAt(cam_x, cam_y, cam_z, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

    // Geometric transformations
    glScalef(scale_x, scale_y, scale_z);

    // Drawing instructions
    glPointSize(point_size);
    glPolygonMode(GL_FRONT_AND_BACK, mode);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);

    for (size_t i = 0; i < vertices.size(); i += 6) {
        if (mode == GL_FILL) glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
        if (mode == GL_FILL) glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertices[i + 1].x(), vertices[i + 1].y(), vertices[i + 1].z());
        if (mode == GL_FILL) glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(vertices[i + 2].x(), vertices[i].y(), vertices[i + 2].z());

        if (mode == GL_FILL) glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(vertices[i + 3].x(), vertices[i + 3].y(), vertices[i + 3].z());
        if (mode == GL_FILL) glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertices[i + 4].x(), vertices[i + 4].y(), vertices[i + 4].z());
        if (mode == GL_FILL) glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(vertices[i + 5].x(), vertices[i + 5].y(), vertices[i + 5].z());
    }
    glEnd();

    if (axis) draw_axis();

    // End of frame
    glutSwapBuffers();
}

void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
            std::cout << "Quitting program" << std::endl;
            exit(0);
        case '1':
            x_axis_len += 0.5f;
            break;
        case '2':
            x_axis_len -= 0.5f;
            break;
        case '3':
            y_axis_len += 0.5f;
            break;
        case '4':
            y_axis_len -= 0.5f;
            break;
        case '5':
            z_axis_len += 0.5f;
            break;
        case '6':
            z_axis_len -= 0.5f;
            break;
        case 'x':
            scale_x -= 0.1f;
            break;
        case 'X':
            scale_x += 0.1f;
            break;
        case 'y':
            scale_y -= 0.1f;
            break;
        case 'Y':
            scale_y += 0.1f;
            break;
        case 'z':
            scale_z -= 0.1f;
            break;
        case 'Z':
            scale_z += 0.1f;
            break;
        case '+':
            scale_x += 0.1f;
            scale_y += 0.1f;
            scale_z += 0.1f;
            break;
        case '-':
            scale_x -= 0.1f;
            scale_y -= 0.1f;
            scale_z -= 0.1f;
            break;
        case 'p':
            mode = GL_POINT;
            break;
        case 'l':
            mode = GL_LINE;
            break;
        case 'f':
            mode = GL_FILL;
            break;
        case 'a':
            alpha -= M_PI / 16;
            break;
        case 'd':
            alpha += M_PI / 16;
            break;
        case 's':
            beta -= M_PI / 16;
            break;
        case 'w':
            beta += M_PI / 16;
            break;
    }
    if (alpha < 0)
        alpha += 2 * M_PI;
    else if (alpha > 2 * M_PI)
        alpha -= 2 * M_PI;

    if (beta < -M_PI)
        beta += M_PI * 2;
    else if (beta > M_PI)
        beta -= M_PI * 2;

    // spherical2cartesian();

    glutPostRedisplay();  // Projects  the changes made at the moment a key is pressed
}

void processSpecialKeys(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_F1:
            point_size += 0.5f;
            break;
        case GLUT_KEY_F2:
            point_size -= 0.5f;
            break;
        case GLUT_KEY_F3:
            axis = !axis;
            break;
        case GLUT_KEY_F4:
            if (!fullscreen)
                glutFullScreen();
            else {
                glutPositionWindow(100, 100);
                glutReshapeWindow(800, 800);
            }
            fullscreen = !fullscreen;
            break;
    }

    glutPostRedisplay();  // Projects  the changes made at the moment a key is pressed
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Invalid number of arguments" << std::endl;
        std::cout << "Use -h option to see what parameters to pass" << std::endl;
        return 1;
    }

    if (!strcmp(argv[1], "-h")) {
        help();
        return 0;
    }

    // Init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG@DI - Engine");

    // Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(processSpecialKeys);

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    std::vector<std::string> models;
    parse_file(argv[1], &models);

    for (size_t i = 0; i < models.size(); ++i) read_file((models_dir + models[i]).c_str());

    // Enter GLUT’s main cycle
    glutMainLoop();

    return 1;
}
