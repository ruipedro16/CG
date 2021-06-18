#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

float cylinder_radius = 1.0f, height = 2.0f;
int sides = 10;

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

void spherical2Cartesian() {
    camX = radius * cos(beta) * sin(alfa);
    camY = radius * sin(beta);
    camZ = radius * cos(beta) * cos(alfa);
}

void cylinder(float radius, float height, int sides) {
    int i;
    float step;

    step = 360.0 / sides;

    glBegin(GL_TRIANGLES);

    // top
    for (i = 0; i < sides; i++) {
        glVertex3f(0, height * 0.5, 0);
        glVertex3f(cos(i * step * M_PI / 180.0) * radius, height * 0.5,
                   -sin(i * step * M_PI / 180.0) * radius);
        glVertex3f(cos((i + 1) * step * M_PI / 180.0) * radius, height * 0.5,
                   -sin((i + 1) * step * M_PI / 180.0) * radius);
    }

    // bottom
    for (i = 0; i < sides; i++) {
        glVertex3f(0, -height * 0.5, 0);
        glVertex3f(cos((i + 1) * step * M_PI / 180.0) * radius, -height * 0.5,
                   -sin((i + 1) * step * M_PI / 180.0) * radius);
        glVertex3f(cos(i * step * M_PI / 180.0) * radius, -height * 0.5,
                   -sin(i * step * M_PI / 180.0) * radius);
    }

    // body
    for (i = 0; i <= sides; i++) {
        glVertex3f(cos(i * step * M_PI / 180.0) * radius, height * 0.5,
                   -sin(i * step * M_PI / 180.0) * radius);
        glVertex3f(cos(i * step * M_PI / 180.0) * radius, -height * 0.5,
                   -sin(i * step * M_PI / 180.0) * radius);
        glVertex3f(cos((i + 1) * step * M_PI / 180.0) * radius, height * 0.5,
                   -sin((i + 1) * step * M_PI / 180.0) * radius);

        glVertex3f(cos(i * step * M_PI / 180.0) * radius, -height * 0.5,
                   -sin(i * step * M_PI / 180.0) * radius);
        glVertex3f(cos((i + 1) * step * M_PI / 180.0) * radius, -height * 0.5,
                   -sin((i + 1) * step * M_PI / 180.0) * radius);
        glVertex3f(cos((i + 1) * step * M_PI / 180.0) * radius, height * 0.5,
                   -sin((i + 1) * step * M_PI / 180.0) * radius);
    }
    glEnd();
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

void renderScene(void) {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

    // put drawing instructions here
    cylinder(cylinder_radius, height, sides);

    // End of frame
    glutSwapBuffers();
}

// write function to process keyboard events
void processKeys(unsigned char c, int xx, int yy) {
    switch (c) {
        case 'q':
            exit(0);
        case 's':
            sides--;
            break;
        case 'S':
            sides++;
            break;
        case 'h':
            height -= 0.1f;
            break;
        case 'H':
            height += 0.1f;
            break;
        case 'r':
            cylinder_radius -= 0.1f;
            break;
        case 'R':
            cylinder_radius += 0.1f;
            break;
    }
    glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            alfa -= 0.1;
            break;
        case GLUT_KEY_LEFT:
            alfa += 0.1;
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
            radius -= 0.1f;
            if (radius < 0.1f) radius = 0.1f;
            break;
        case GLUT_KEY_PAGE_UP:
            radius += 0.1f;
            break;
    }
    spherical2Cartesian();
    glutPostRedisplay();
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

    // put here the registration of the keyboard callbacks
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

    //  OpenGL settings
    glPolygonMode(GL_FRONT, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    spherical2Cartesian();

    // enter GLUT's main cycle
    glutMainLoop();

    return 1;
}
