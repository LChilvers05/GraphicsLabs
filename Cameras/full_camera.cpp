/****************************************************************************
*
* krt - Ken's Raytracer - Coursework Edition. (C) Copyright 1993-2023.
*
* I've put a lot of time and effort into this code. For the last decade
* it's been used to introduce hundreds of students at multiple universities
* to raytracing. It forms the basis of your coursework but you are free
* to continue using/developing forever more. However, I ask that you don't
* share the code or your derivitive versions publicly. In order to continue
* to be used for coursework and in particular assessment it's important that
* versions containing solutions are not searchable on the web or easy to
* download.
*
* If you want to show off your programming ability, instead of releasing
* the code, consider generating an incredible image and explaining how you
* produced it.
*/

#include "full_camera.h"

FullCamera::FullCamera() {
    fov = 0.5f;
}

FullCamera::FullCamera(float f, Vertex& p_position, Vector& p_lookat, Vector& p_up) {
    fov = f;
    position = p_position;
    lookat = p_lookat;
    up = p_up;
}

void FullCamera::get_ray_offset(int p_x, int p_y, float p_ox, float p_oy, Ray& p_ray) {

}

void FullCamera::get_ray_pixel(int p_x, int p_y, Ray &ray) {
    //construct an orthonormal basis
    float e_l = (float)1/(position.operator-(lookat).length());

    Vector w = position.operator-(lookat.operator*(e_l));

    Vector w_cross_up = Vector();
    w.cross(up, w_cross_up);
    float w_cross_up_length = w_cross_up.length();

    Vector u = w_cross_up.operator*(w_cross_up_length);

    Vector v = Vector();
    w.cross(u, v);

    //TODO: create a direction of ray
    //TODO: manipulate ray to be returned.
}

void FullCamera::render(Environment& env, FrameBuffer& fb) {
    width = fb.width;
    height = fb.height;

    //for each pixel
    for (int y = 0; y < height; y +=1) {
        for (int x = 0; x < width; x +=1) {
            Ray ray;

            // get_ray_pixel();

            Colour colour;
            float depth;


        }
    }
}
