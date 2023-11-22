#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// these are core raytracing classes
#include "framebuffer.h"
#include "scene.h"

// classes that contain our objects to be rendered, all derived from Object
#include "polymesh_object.h"
#include "sphere_object.h"
#include "plane_object.h"
#include "quadratic_object.h"

// classes that contain our lights, all derived from Light
#include "directional_light.h"

// classes that contain the materials applied to an object, all derived from Material
#include "global_material.h"
#include "phong_material.h"
#include "falsecolour_material.h"

//classes that contain cameras, all derived from Camera
#include "depth_of_field_camera.h"

using namespace std;

Light* make_light(Vector& ldir) {
	ldir.normalise();
	Colour lColour = Colour(255.0f, 255.0f, 255.0f);
	Light* light = new DirectionalLight(ldir, lColour);
	return light;
}

void build_scene(Scene& scene) {
    Vector left_light_dir = Vector(1.f, -1.f, 1.f);
	scene.add_light(make_light(left_light_dir));

    Phong* blue = new Phong(Colour(0.f, 0.f, 255.f));
    Phong* red = new Phong(Colour(255.f, 0.f, 0.f));
	Phong* green = new Phong(Colour(0.f, 255.f, 0.f));

    Sphere* blue_sphere = new Sphere(Vertex(0.f, 0.f, 0.f), 5.f);
    blue_sphere->set_material(blue);
    scene.add_object(blue_sphere);

    Sphere* red_sphere = new Sphere(Vertex(10.f, 0.f, 5.f), 5.f);
    red_sphere->set_material(red);
    scene.add_object(red_sphere);

	Sphere* green_sphere = new Sphere(Vertex(-10.f, 2.f, 2.f), 3.f);
    green_sphere->set_material(green);
    scene.add_object(green_sphere);
}

// This is the entry point function to the program.
int main(int argc, char *argv[]) {
	// Create a framebuffer
	FrameBuffer* fb = new FrameBuffer(512, 512);
	
	// Create a scene
	Scene scene;
	
	// Setup the scene
	build_scene(scene);

	// look down y-axis
	Vertex p_position = Vertex(0.f, 0.f, -20.f);
	Vector p_lookat = Vector(0.f, 0.f, 1.f);
	Vector p_up = Vector(0.f, 1.f, 0.f);
	Camera *camera = new DepthOfFieldCamera(0.6f, 20.f, 3.f, p_position, p_lookat, p_up);
	
	// Camera generates rays for each pixel in the framebuffer and records colour + depth.
	camera->render(scene,*fb);
	
	// Output the framebuffer colour and depth as two images
	fb->writeRGBFile((char *)"depth_of_field.ppm");
	// fb->writeDepthFile((char *)"depth.ppm");
	
	cerr << "\nDone.\n" << flush;
	
	// a debug check to make sure we don't leak hits.
	cerr << "Hit Pool " << Hit::pool_size << " Allocated " << Hit::allocated << "\n" << flush;
	return 0;
}
