#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "camera.hpp"
#include "group.hpp"
#include "parser.hpp"
#include "transform.hpp"

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

float eTime = 0.0f, cTime = 0.0f, speed = 1.0f;
bool stop = false;

void drawOrbits(Transform *t) {
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_LINE_LOOP);

  for (Vertex *p : t->getPointsCurve()) {
    glVertex3f(p->getX(), p->getY(), p->getZ());
  }
  glEnd();
}

void apply_trans(Transform *t) {
  float cTimeAux = glutGet(GLUT_ELAPSED_TIME);
  if (!stop)
    eTime += (cTimeAux - cTime) * speed;
  cTime = cTimeAux;

  const char *type = t->getType().c_str();
  float x = t->getX();
  float y = t->getY();
  float z = t->getZ();
  float angle = t->getAngle();
  float time = t->getTime();

  if (!strcmp(type, "translation"))
    glTranslatef(x, y, z);

  else if (!strcmp(type, "scale"))
    glScalef(x, y, z);

  else if (!strcmp(type, "rotation"))
    glRotatef(angle, x, y, z);

  else if (!strcmp(type, "colour"))
    glColor3f(x, y, z);

  else if (!strcmp(type, "rotateTime")) {
    float aux = eTime * angle;
    glRotatef(aux, x, y, z);
  }

  else if (!strcmp(type, "translateTime")) {
    float p[4], d[4];
    float dTime = eTime * time;

    t->getGlobalCatmullRomPoint(dTime, p, d);

    drawOrbits(t);
    glTranslatef(p[0], p[1], p[2]);

    if (t->isDeriv()) {
      float res[4];
      t->normalize(d);
      t->cross_product(d, t->getVector(), res);
      t->normalize(res);
      t->cross_product(res, d, t->getVector());
      float matrix[16];
      t->normalize(t->getVector());
      t->rotMatrix(matrix, d, t->getVector(), res);
      glMultMatrixf(matrix);
    }
  }
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
  cout << endl;
  cout << "\ts: Stop/Resume the animation" << endl;
  cout << "\t1: Speed up the animation" << endl;
  cout << "\t2: Slow down the animation" << endl;
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

  for (Transform *t : scene->getTransforms())
    apply_trans(t);

  for (Shape *shape : scene->getShapes())
    shape->drawShape();

  for (Group *g : scene->getGroups())
    drawScene(g);

  glPopMatrix();
}

void renderScene(void) {
  // Clear buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set camera
  glLoadIdentity();
  gluLookAt(camera->getCamX(), camera->getCamY(), camera->getCamZ(),
            camera->getLookX(), camera->getLookY(), camera->getLookZ(), 0.0f,
            1.0f, 0.0f);

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
  if (h == 0)
    h = 1;

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
  case 's':
    stop = !stop;
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
  case '1':
    speed += 0.25f;
    break;
  case '2':
    if (speed > 0.5f)
      speed -= 0.25f;
    break;
  default:
    camera->keys(key);
    break;
  }
  glutPostRedisplay(); // Projects the changes made at the moment a key is
                       // pressed
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
  glutPostRedisplay(); // Projects the changes made at the moment a key is
                       // pressed
}

void mouseButton(int button, int state, int x, int y) {
  camera->mouseButton(button, state, x, y);
}

void mouseMove(int x, int y) { camera->mouseMove(x, y); }

void processMenu(int option) {
  switch (option) {
  case 0:
    exit(0);
  case 1:
    camera->initialPosition(); // sun (origin)
    break;
  case 2:
    stop = true;
    break;
  case 3:
    stop = false;
    break;
  }
  glutPostRedisplay(); // Projects the changes
}

void showMenu() {
  glutCreateMenu(processMenu);
  glutAddMenuEntry("Exit", 0);
  glutAddMenuEntry("Reset Camera Position", 1);
  glutAddMenuEntry("Stop The Animation", 2);
  glutAddMenuEntry("Resume The Animation", 3);
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

  // Init GLUT and the window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(800, 800);
  glutCreateWindow("CG@DI-UM - Engine");

#ifndef __APPLE__
  if (glewInit() != GLEW_OK) {
    return 1;
  }
#endif

  camera = new Camera();
  scene = load_XML_file(argv[1], &vertices);
  if (scene == nullptr)
    return 1;

  // Required callback registry
  glutDisplayFunc(renderScene);
  glutReshapeFunc(changeSize);
  glutIdleFunc(renderScene);

  glutKeyboardFunc(keyboardFunc);
  glutSpecialFunc(processSpecialKeys);

  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMove);

  showMenu();

  // OpenGL settings
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnableClientState(GL_VERTEX_ARRAY);

  // Enter GLUT’s main cycle
  timebase = glutGet(GLUT_ELAPSED_TIME);
  glutMainLoop();

  return 1;
}
