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
    up = p_up; up.normalise();

    Vertex lookat_point = position + lookat;
    make_orthonormal_bases(position, lookat_point, up);
}

void FullCamera::make_orthonormal_bases(Vertex& eye, Vertex& look, Vector& up) {
    //w = (eye - look) / |eye - look|
    float e_l = (1.0f/(eye - look).length());
    this->w = (eye - look) * e_l;
    //u = (w X up) / (|w X up|)
    Vector u = Vector(); w.cross(up, u); u.normalise();
    this->u = u;
    //v = w X u
    w.cross(u, this->v);
}

void FullCamera::get_ray_pixel(int p_x, int p_y, Ray& ray) {    
    //each ray begins at the eye point
    ray.position = position;
    ray.position.w = 1.0f;

    float s = 1.0f;
    float fx = s*((float)p_x) - (width/2.0f);
    float fy = s*((float)p_y) - (height/2.0f);

    //distance
    float d = (height/2.0f)/tan(fov);

    //create direction vector from orthonormal basis
    ray.direction = (u * fx) + (v * fy) - (w * d);
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
