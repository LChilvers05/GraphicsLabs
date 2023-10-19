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
    lookat = p_lookat; lookat.normalise();
    //need look point not direction
    lookat_point = position.operator+(lookat);
    up = p_up; up.normalise();
    make_orthonormal_bases(position, lookat_point, up);
}

void FullCamera::make_orthonormal_bases(Vertex& eye, Vertex& look, Vector& up) {
    //w = eye - (look / |eye - look|)
    float e_l = 1.0f/(eye.operator-(look).length());
    w = eye.operator-(look.operator*(e_l));
    //u = (w X up) / (|w X up|)
    Vector w_X_up = Vector();
    w.cross(up, w_X_up);
    float w_X_up_len = 1.0f/w_X_up.length();
    u = w_X_up.operator*(w_X_up_len);
    //v = w X u
    w.cross(u, v);
}

void FullCamera::get_ray_offset(int p_x, int p_y, float p_ox, float p_oy, Ray& p_ray) {

}

void FullCamera::get_ray_pixel(int p_x, int p_y, Ray& ray) {    
    //each ray begins at the eye point
    ray.position = position;
    ray.position.w = 1.0f;

    float fx = ((float)p_x) - (width/2.0f);
    float fy = ((float)p_y) - (height/2.0f);

    //distance
    float d = (height/2.0f)/tan(fov);

    //create direction vector from orthonormal basis
    Vector x_u = u.operator*(fx);
    Vector y_v = v.operator*(fy);
    Vector d_w = w.operator*(d);

    Vector x_u_y_v = x_u.operator+(y_v);
    ray.direction.x = x_u_y_v.x - d_w.x;
    ray.direction.y = x_u_y_v.y - d_w.y;
    ray.direction.z = x_u_y_v.z - d_w.z;
    ray.direction.normalise();
}

void FullCamera::render(Environment& env, FrameBuffer& fb) {
    width = fb.width;
    height = fb.height;

    //for each pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Ray ray;

            get_ray_pixel(x, y, ray);

            Colour colour;
            float depth;
            
            env.raytrace(ray, 5, colour, depth);

            fb.plotPixel(x, y, colour.r, colour.g, colour.b);
			fb.plotDepth(x, y, depth);
        }

        cerr << "#" << flush;
    }
}
