#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <vector>

using std::vector;

bool fullscreen = false;

float alpha = 0.0f, beta = 0.5f, radius = 100.0f;
float cam_x, cam_y, cam_z;

int r = 50, ri = 35, rc = 15;

vector<float> trees;

void spherical2Cartesian() {
    cam_x = radius * cos(beta) * sin(alpha);
    cam_y = radius * sin(beta);
    cam_z = radius * cos(beta) * cos(alpha);
}

void tree_pos(int n_trees, int min_radius, int max_radius) {
    for (int i = 0; i < n_trees; i++) {
        // rand() % (max_number + 1 - minimum_number) + minimum_number
        int delta = 5;
        int min_number = -max_radius + delta, max_number = max_radius - delta;
        float x, z;
        do {
            x = rand() % (max_number + 1 - min_number) + min_number;
            z = rand() % (max_number + 1 - min_number) + min_number;
        } while (sqrt(pow(x, 2) + pow(z, 2)) < min_radius + delta);
        trees.push_back(x);
        trees.push_back(z);
    }
}

void changeSize(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if (h == 0) h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void draw_trees(int n_trees, vector<float> pos) {
    for (int i = 0; i < pos.size(); i += 2) {
        glPushMatrix();
        // trunk
        glTranslatef(pos[i], 0.0f, pos[i + 1]);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glColor3f(0.4f, 0.243f, 0.141f);
        glutSolidCone(0.5, 3, 15, 15);

        // leaves
        glTranslatef(0.0f, 0.0f, 3.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glutSolidCone(2, 4, 10, 10);

        glPopMatrix();
    }
}

void renderScene(void) {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(cam_x, cam_y, cam_z, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

    glColor3f(0.2f, 0.8f, 0.2f);
    glBegin(GL_TRIANGLES);
    glVertex3f(100.0f, 0, -100.0f);
    glVertex3f(-100.0f, 0, -100.0f);
    glVertex3f(-100.0f, 0, 100.0f);

    glVertex3f(100.0f, 0, -100.0f);
    glVertex3f(-100.0f, 0, 100.0f);
    glVertex3f(100.0f, 0, 100.0f);
    glEnd();
    // End of frame

    // put code to draw scene in here
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidTorus(1, 3, 15, 15);

    glColor3f(0.0f, 0.0f, 1.0f);
    for (int i = 0; i < 8; i++) {
        glPushMatrix();
        glRotatef(360.0f / 8 * i, 0.0f, 1.0f, 0.0f);
        glTranslatef(rc, 1.0f, 1.0f);
        glutSolidTeapot(2);
        glPopMatrix();
    }

    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < 16; i++) {
        glPushMatrix();
        glRotatef(360.0f / 16 * i, 0.0f, 1.0f, 0.0f);
        glTranslatef(ri, 1.0f, 0.0f);
        glutSolidTeapot(2);
        glPopMatrix();
    }

    draw_trees(100, trees);

    glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {
    // put code to process regular keys in here
    switch (c) {
        case 'q':
            exit(0);
        case '+':
            radius -= 1.5f;
            if (radius < 1.0f) radius = 1.0f;
            break;
        case '-':
            radius += 1.5f;
            break;
    }
    spherical2Cartesian();
    glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            alpha -= 0.1;
            break;

        case GLUT_KEY_LEFT:
            alpha += 0.1;
            break;

        case GLUT_KEY_UP:
            beta += 0.1f;
            if (beta > 1.5f) beta = 1.5f;
            break;

        case GLUT_KEY_DOWN:
            beta -= 0.1f;
            if (beta < -1.5f) beta = -1.5f;
            break;

        case GLUT_KEY_PAGE_DOWN:
            radius -= 1.0f;
            if (radius < 1.0f) radius = 1.0f;
            break;

        case GLUT_KEY_PAGE_UP:
            radius += 1.0f;
            break;
        case GLUT_KEY_F1:
            if (!fullscreen)
                glutFullScreen();
            else {
                glutPositionWindow(100, 100);
                glutReshapeWindow(800, 800);
            }
            fullscreen = !fullscreen;
            break;
    }
    spherical2Cartesian();
    glutPostRedisplay();
}

void printInfo() {
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("\nUse Arrows to move the camera up/down and left/right\n");
    printf("Home and End control the distance from the camera to the origin");
}

int main(int argc, char **argv) {
    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG@DI-UM");

    // Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    // Callback registration for keyboard processing
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    spherical2Cartesian();

    printInfo();

    tree_pos(100, ri, 100);

    // enter GLUT's main cycle
    glutMainLoop();

    return 1;
}
