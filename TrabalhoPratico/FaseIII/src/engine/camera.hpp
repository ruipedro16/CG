#ifndef __CAMERA_H__
#define __CAMERA_H__

class Camera {
private:
  float alpha, beta, radius;
  float cam_x, cam_y, cam_z;
  float look_x, look_y, look_z, mousePosX, mousePosY;
  bool mousePressed;

public:
  Camera();

  float getCamX() const;

  float getCamY() const;

  float getCamZ() const;

  float getLookX() const;

  float getLookY() const;

  float getLookZ() const;

  void initialPosition();

  void change_look(float x, float y, float z);

  void keys(unsigned char key);

  void special_keys(int key);

  void mouseButton(int button, int state, int x, int y);

  void mouseMove(int x, int y);
};

#endif
