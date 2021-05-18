#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

GLfloat t_x = 0.0f, t_y = 0.0f, t_z = 0.0f;
GLfloat scale_x = 1.0f, scale_y = 1.0f, scale_z = 1.0f;
GLfloat angle = 0.0f;

GLenum mode = GL_FILL;

GLfloat alpha = 0.0f, beta = 0.5f, radius = 5.0f;
GLfloat cam_x = radius * cos(beta) * sin(alpha);
GLfloat cam_y = radius * sin(beta);
GLfloat cam_z = radius * cos(beta) * cos(alpha);

void spherical2Cartesian() {
    cam_x = radius * cos(beta) * sin(alpha);
    cam_y = radius * sin(beta);
    cam_z = radius * cos(beta) * cos(alpha);
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
    gluLookAt(cam_x, cam_y, cam_z, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

    // put the geometric transformations here
    glScalef(scale_x, scale_y, scale_x);
    glTranslatef(t_x, t_y, t_z);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    // put drawing instructions here
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    glBegin(GL_TRIANGLES);

    // Base
    // First Triangle
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);

    // Second Triangle
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);

    // Front
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 1.5f, 0.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);

    // Right
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 1.5f, 0.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);

    // Back
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 1.5f, 0.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);

    // Left
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.5f, 0.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glEnd();

    // End of frame
    glutSwapBuffers();
}

// write function to process keyboard events

void processKeys(unsigned char key, int xx, int yy) {
    switch (key) {
        case 'q':
            exit(0);
        case 'l':
            mode = GL_LINE;
            break;
        case 'f':
            mode = GL_FILL;
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
        case 'a':
            t_x -= 0.1;
            break;
        case 'd':
            t_x += 0.1;
            break;
        case 's':
            t_z += 0.1;
            break;
        case 'w':
            t_z -= 0.1;
            break;
        case '1':
            scale_x += 0.1;
            break;
        case '2':
            scale_x -= 0.1;
            break;
        case '3':
            scale_y -= 0.1;
            break;
        case '4':
            scale_y += 0.1;
            break;
        case '5':
            scale_z -= 0.1;
            break;
        case '6':
            scale_z += 0.1;
            break;
    }
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
            radius -= 0.1f;
            if (radius < 0.1f) radius = 0.1f;
            break;
        case GLUT_KEY_PAGE_UP:
            radius += 0.1f;
            break;
        case GLUT_KEY_F1:
            angle += 15.0f;
            break;
        case GLUT_KEY_F2:
            angle -= 15.0f;
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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // enter GLUT's main cycle
    glutMainLoop();

    return 1;
}
