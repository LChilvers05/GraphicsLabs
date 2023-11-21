#include "depth_of_field_camera.h"

#include <iostream>
#include <random>

DepthOfFieldCamera::DepthOfFieldCamera() {
    fov = 0.5f;
}

DepthOfFieldCamera::DepthOfFieldCamera(
    float p_f, 
    float p_focal_length,
    float p_aperature,
    Vertex& p_position, 
    Vector& p_lookat, 
    Vector& p_up
) {
    fov = p_f;
    focal_length = p_focal_length;
    aperature = p_aperature;
    position = p_position;
    lookat = p_lookat; lookat.normalise();
    up = p_up; up.normalise();

    Vertex lookat_point = position + lookat;
    make_orthonormal_bases(position, lookat_point, up);
}

void DepthOfFieldCamera::make_orthonormal_bases(Vertex& eye, Vertex& look, Vector& up) {
    //w = (eye - look) / |eye - look|
    float e_l = (1.0f/(eye - look).length());
    this->w = (eye - look) * e_l;
    //u = (w X up) / (|w X up|)
    Vector u = Vector(); w.cross(up, u); u.normalise();
    this->u = u;
    //v = w X u
    w.cross(u, this->v);
}

void DepthOfFieldCamera::get_ray_pixel(int p_x, int p_y, Ray& ray) {    
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

// https://pathtracing.home.blog/depth-of-field/

void DepthOfFieldCamera::render(Environment& env, FrameBuffer& fb) {
    width = fb.width;
    height = fb.height;

    //for each pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            // primary ray
            Ray ray;
            get_ray_pixel(x, y, ray);

            Colour colour;
            float depth;
            env.raytrace(ray, 5, colour, depth);

            Colour average;

            Vertex convergence = ray.position + (focal_length*ray.direction);
            
            const int sample_size = 50;
            for (int i = 0; i < sample_size; i++) {
                // generate random number between aperature
                std::random_device rand;
                std::mt19937 range(rand());
                std::uniform_real_distribution<float> distribution(-aperature, aperature);
                float r = distribution(range);
                
                Ray sample_ray;
                // starting point of sample ray within range of aperature
                sample_ray.position = ray.position + (r*up);
                sample_ray.position.w = 1.f;
                sample_ray.direction = convergence - sample_ray.position;
                sample_ray.direction.normalise();

                Colour v_colour; float v_depth;
                env.raytrace(sample_ray, 5, v_colour, v_depth);
                average = average + v_colour;

                Vector right = lookat; right.cross(up); right.normalise();

                Ray h_ray;
                h_ray.position = ray.position + (r*right);
                h_ray.position.w = 1.f;
                h_ray.direction = convergence - h_ray.position;
                h_ray.direction.normalise();

                Colour h_colour; float h_depth;
                env.raytrace(h_ray, 5, h_colour, h_depth);
                average = average + h_colour;
            }
            // average colour of the sample
            average = (average + colour) * (1.f/(2.f * sample_size + 1.f));

            fb.plotPixel(x, y, average.r, average.g, average.b);
			fb.plotDepth(x, y, depth);
        }

        cerr << "#" << flush;
    }
}