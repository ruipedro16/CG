#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <cstring>
#include <iostream>
#include <vector>

#include "camera.hpp"
#include "group.hpp"
#include "parser.hpp"

using namespace std;

// Global Variables
bool fullscreen = false;
GLfloat point_size = 1.0f;
GLenum mode = GL_LINE;

Camera *camera;
Group *scene;
vector<Vertex *> vertices;

int timebase;
float frames;

string planet_name(int i) {
    string s;
    switch (i) {
        case 1:
            s = "Mercury";
            break;
        case 2:
            s = "Venus";
            break;
        case 3:
            s = "Earth";
            break;
        case 4:
            s = "Mars";
            break;
        case 5:
            s = "Jupiter";
            break;
        case 6:
            s = "Saturn";
            break;
        case 7:
            s = "Uranus";
            break;
        case 8:
            s = "Neptune";
            break;
        case 9:
            s = "Pluto";
            break;
    }
    return s;
}

void help() {
    cout << "Usage: ./engine [-h] <XML file>" << endl;
    cout << "\tF1: Increase point size" << endl;
    cout << "\tF2: Decrease point size" << endl;
    cout << "\tF3: Enable/Disable fullscreen" << endl;
    cout << "\tF4: Reset camera position" << endl;
    cout << endl;
    cout << "\t+: Zoom in" << endl;
    cout << "\t-: Zoom out" << endl;
    cout << "\tKEY  UP: Move the camera position upwards" << endl;
    cout << "\tKEY  DOWN: Move the camera position downwards" << endl;
    cout << "\tKEY  LEFT: Move the camera position to the left" << endl;
    cout << "\tKEY  RIGHT: Move the camera position to the right" << endl;
    cout << endl;
    cout << "\tp: Change the figure format into points" << endl;
    cout << "\tl: Change the figure format into lines" << endl;
    cout << "\tf: Fill up the figure" << endl;
    cout << "\tq: Exit" << endl;
}

void framerate() {
    char title[50];
    frames++;
    double time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        double fps = frames * 1000.0 / (time - timebase);
        timebase = time;
        frames = 0;
        sprintf(title, "CG@DI-UM - Engine | %.2lf FPS", fps);
        glutSetWindowTitle(title);
    }
}

void drawScene(Group *scene) {
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);

    for (Transform *t : scene->getTransforms()) {
        const char *type = t->getType().c_str();
        if (!strcmp(type, "translation"))
            glTranslatef(t->getX(), t->getY(), t->getZ());
        else if (!strcmp(type, "rotation"))
            glRotatef(t->getAngle(), t->getX(), t->getY(), t->getZ());
        else if (!strcmp(type, "scale"))
            glScalef(t->getX(), t->getY(), t->getZ());
        else if (!strcmp(type, "colour"))
            glColor3f(t->getX(), t->getY(), t->getZ());
    }

    glBegin(GL_TRIANGLES);
    for (Shape *shape : scene->getShapes())
        for (Vertex *v : shape->getVertices()) glVertex3f(v->getX(), v->getY(), v->getZ());
    glEnd();

    for (Group *g : scene->getGroups()) drawScene(g);

    glPopMatrix();
}

void renderScene(void) {
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set camera
    glLoadIdentity();
    gluLookAt(camera->getCamX(), camera->getCamY(), camera->getCamZ(), camera->getLookX(),
              camera->getLookY(), camera->getLookZ(), 0.0f, 1.0f, 0.0f);

    // Drawing instructions
    glPointSize(point_size);
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    drawScene(scene);
    framerate();

    // End of frame
    glutSwapBuffers();
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

void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
            exit(0);
        case 'p':
            mode = GL_POINT;
            break;
        case 'l':
            mode = GL_LINE;
            break;
        case 'f':
            mode = GL_FILL;
            break;
        default:
            camera->keys(key);
            break;
    }
    glutPostRedisplay();  // Projects the changes made at the moment a key is pressed
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
            if (!fullscreen)
                glutFullScreen();
            else {
                glutPositionWindow(100, 100);
                glutReshapeWindow(800, 800);
            }
            fullscreen = !fullscreen;
            break;
        default:
            camera->special_keys(key);
            break;
    }
    glutPostRedisplay();  // Projects the changes made at the moment a key is pressed
}

void processMenu(int option) {
    switch (option) {
        case 0:
            exit(0);
        case 1:
            camera->initialPosition();  // sun (origin)
            break;
        default: {
            Vertex *v = vertices.at(option - 2);
            camera->change_look(v->getX(), v->getY(), v->getZ());
            break;
        }
    }
    glutPostRedisplay();  // Projects the changes
}

void showMenu() {
    int planet = glutCreateMenu(processMenu);

    glutAddMenuEntry("Sun", 1);

    for (size_t i = 0; i < vertices.size(); i++) {
        char str[10];
        sprintf(str, planet_name(i + 1).c_str(), i + 1);
        glutAddMenuEntry(str, i + 2);
    }

    glutCreateMenu(processMenu);
    glutAddSubMenu("Planet ", planet);
    glutAddMenuEntry("Exit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Invalid number of arguments" << endl;
        cout << "Use -h option to see what parameters to pass" << endl;
        return 1;
    }

    if (!strcmp(argv[1], "-h")) {
        help();
        return 0;
    }

    camera = new Camera();
    scene = load_XML_file(argv[1], &vertices);
    if (scene == nullptr) return 1;

    // Init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG@DI-UM - Engine");

    // Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(processSpecialKeys);
    showMenu();

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Enter GLUT’s main cycle
    timebase = glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop();

    return 1;
}
