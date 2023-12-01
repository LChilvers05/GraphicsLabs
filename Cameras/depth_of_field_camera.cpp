#include "depth_of_field_camera.h"

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
    right = lookat; right.cross(up); right.normalise();

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

Ray DepthOfFieldCamera::get_primary_ray(int p_x, int p_y) { 
    Ray ray;
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

    return ray;
}

Ray DepthOfFieldCamera::get_secondary_ray(Ray& primary_ray) {

    // generate random multipliers for direction
    float r1 = random_float(-aperature, aperature);
    float r2 = random_float(-aperature, aperature);

    // point where all secondary (and primary) rays will meet
    Vertex convergence = primary_ray.position + (focal_length * primary_ray.direction);

    Ray ray;
    // starting point of sample ray within range of aperature
    ray.position = primary_ray.position + (r1*up) + (r2*right);
    ray.position.w = 1.f;
    ray.direction = convergence - ray.position;
    ray.direction.normalise();

    return ray;
}

// https://pathtracing.home.blog/depth-of-field/

void DepthOfFieldCamera::render(Environment& env, FrameBuffer& fb) {
    width = fb.width;
    height = fb.height;

    //for each pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            
            // ray trace with primary
            Colour colour; float depth;
            Ray primary_ray = get_primary_ray(x, y);
            env.raytrace(primary_ray, 5, colour, depth);

            // ray trace with sample for depth of field
            const int sample_size = 500;
            for (int i = 0; i < sample_size; i++) {

                Colour sample_colour; float depth;
                Ray secondary_ray = get_secondary_ray(primary_ray);

                env.raytrace(secondary_ray, 5, sample_colour, depth);
                colour = colour + sample_colour;
            }
            // average colour of the sample
            colour = colour * (1.f/(sample_size + 1));

            fb.plotPixel(x, y, colour.r, colour.g, colour.b);
			fb.plotDepth(x, y, depth);
        }

        cerr << "#" << flush;
    }
}