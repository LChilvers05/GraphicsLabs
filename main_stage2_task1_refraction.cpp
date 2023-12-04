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

// classes that contain our lights, all derived from Light
#include "directional_light.h"

// classes that contain the materials applied to an object, all derived from Material
#include "global_material.h"
#include "phong_material.h"

//classes that contain cameras, all derived from Camera
#include "simple_camera.h"
#include "full_camera.h"

using namespace std;

Sphere* make_sphere(const Vertex c, float r, Material* m) {
	Sphere* s = new Sphere(c,r);
	s->set_material(m);
	return s;
}

Light* make_light(Vector& ldir) {
	ldir.normalise();
	Colour lColour = Colour(255.0f, 255.0f, 255.0f);
	Light* light = new DirectionalLight(ldir, lColour);
	return light;
}

Plane* make_floor() {
	Vector normal = Vector(0.f, 1.f, -0.0001f);
	Plane* floor = new Plane(
		normal.x,
		normal.y,
		normal.z,
		-(normal.z * 20.f)
	);
	return floor;
}

void build_scene(Scene& scene) {
	Vector left_light_dir = Vector(1.f, -1.f, 1.f);
	scene.add_light(make_light(left_light_dir));

    GlobalMaterial* mirror = new GlobalMaterial(
		Colour(100.f, 100.f, 100.f),
		&scene, 
		Colour(0.3f, 0.3f, 0.3f), 
		Colour(0.f, 0.f, 0.f), 
		0.f,
		false
	);

    GlobalMaterial* glass = new GlobalMaterial(
		Colour(50.f, 50.f, 50.f),
		&scene, 
		Colour(0.1f, 0.1f, 0.1f), 
		Colour(0.6f, 0.6f, 0.6f), 
		1.52f,
		false
	);
    Phong* blue = new Phong(Colour(0.f, 0.f, 255.f));
    Phong* red = new Phong(Colour(255.f, 0.f, 0.f));

	Plane* floor = make_floor();
	floor->set_material(mirror);
	scene.add_object(floor);

	Sphere* glass_sphere = make_sphere(Vertex(0.f, 10.f, 4.f), 8.f, glass);
    scene.add_object(glass_sphere);

    // right
    Sphere* blue_sphere = make_sphere(Vertex(9.f, 10.f, 21.f), 8.f, blue);
    scene.add_object(blue_sphere);

    // left
    Sphere* red_sphere = make_sphere(Vertex(-9.f, 10.f, 21.f), 8.f, red);
    scene.add_object(red_sphere);
}

// This is the entry point function to the program.
int main(int argc, char *argv[]) {
	// Create a framebuffer
	FrameBuffer* fb = new FrameBuffer(512, 512);
	
	// Create a scene
	Scene scene;
	
	// Setup the scene
	build_scene(scene);

	Vertex p_position = Vertex(0.01f, 10.0f, -22.0f);
	Vector p_lookat = Vector(-0.01f, -0.1f, 1.0f);
	Vector p_up = Vector(0.0f, 1.0f, 0.0f);
	Camera *camera = new FullCamera(0.6f, p_position, p_lookat, p_up);
	
	// Camera generates rays for each pixel in the framebuffer and records colour + depth.
	camera->render(scene,*fb);
	
	// Output the framebuffer colour and depth as two images
	fb->writeRGBFile((char *)"final_refraction.ppm");
	// fb->writeDepthFile((char *)"depth.ppm");
	
	cerr << "\nDone.\n" << flush;
	
	// a debug check to make sure we don't leak hits.
	cerr << "Hit Pool " << Hit::pool_size << " Allocated " << Hit::allocated << "\n" << flush;
	return 0;
}
