#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include "camera.hpp"

Camera::Camera() { initialPosition(); }

float Camera::getCamX() const { return cam_x; }

float Camera::getCamY() const { return cam_y; }

float Camera::getCamZ() const { return cam_z; }

float Camera::getLookX() const { return look_x; }

float Camera::getLookY() const { return look_y; }

float Camera::getLookZ() const { return look_z; }

void Camera::initialPosition() {
    alpha = M_PI, beta = M_PI / 5, radius = 100.0f;
    look_x = 0.0f, look_y = 0.0f, look_z = 1.0f;
    cam_x = look_x + radius * cos(beta) * sin(alpha);
    cam_y = look_y + radius * sin(beta);
    cam_z = look_z + radius * cos(beta) * cos(alpha);
}

void Camera::change_look(float x, float y, float z) {
    initialPosition();
    look_x = x, look_y = y, look_z = z;
    cam_x += x, cam_y += y, cam_z += z;
}

void Camera::keys(unsigned char key) {
    switch (key) {
        case '-':
            radius += 0.7f;
            break;
        case '+':
            radius -= 0.7f;
            break;
    }
    cam_x = look_x + radius * cos(beta) * sin(alpha);
    cam_y = look_y + radius * sin(beta);
    cam_z = look_z + radius * cos(beta) * cos(alpha);
}

void Camera::special_keys(int key) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            alpha += 0.1f;
            break;
        case GLUT_KEY_LEFT:
            alpha -= 0.1f;
            break;
        case GLUT_KEY_UP:
            beta += 0.1f;
            if (beta > 1.5f) beta = 1.5f;
            break;
        case GLUT_KEY_DOWN:
            beta -= 0.1f;
            if (beta < -1.5f) beta = -1.5f;
            break;
        case GLUT_KEY_F4:
            initialPosition();
            break;
    }
    cam_x = look_x + radius * cos(beta) * sin(alpha);
    cam_y = look_y + radius * sin(beta);
    cam_z = look_z + radius * cos(beta) * cos(alpha);
}