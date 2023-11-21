#ifndef _FULL_CAMERA_H_
#define _FULL_CAMERA_H_

#include "camera.h"

class DepthOfFieldCamera: public Camera {
public:
  int width;
  int height;
  float fov;
  float focal_length;
  float aperature;
  Vertex position;
  Vector lookat;
  Vector up;

  DepthOfFieldCamera();

  DepthOfFieldCamera(float p_f, float p_focal_length, float p_aperature, Vertex& p_position, Vector& p_lookat, Vector &p_up);

  void get_ray_offset(int p_x, int p_y, float p_ox, float p_oy, Ray &p_ray);

  void get_ray_pixel(int p_x, int p_y, Ray &ray);

  void render(Environment& env, FrameBuffer& fb);

private:
  //orthonormal basis
  Vector w;
  Vector u;
  Vector v;

  void make_orthonormal_bases(Vertex& eye, Vertex& look, Vector& up);

};

#endif