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

Sphere* make_sphere(const Vertex c, const float r) {
	return new Sphere(c, r);
}

PolyMesh* make_teapot() {
	PolyMesh* teapot = new PolyMesh((char *)"teapot-low.obj", false);
	teapot->smooth_render = true;
	Transform * transform = new Transform(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(90.0f), sin(90.0f), 0.0f,
		0.0f, -sin(90.0f), cos(90.0f), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
  	); 
	teapot->apply_transform(*transform);
	return teapot;
}

Light* make_light() {
	Vector ldir = Vector(-3.0f, -0.25f, 1.0f); ldir.normalise();
	Colour lColour = Colour(255.0f, 255.0f, 255.0f);
	Light* light = new DirectionalLight(ldir, lColour);
	return light;
}

void build_scene(Scene& scene) {
	GlobalMaterial* mat = new GlobalMaterial();
  	Object* object = make_teapot();
	object->set_material(mat);
	
	scene.add_object(object);
	scene.add_light(make_light());
}

void build_scene_lab_4(Scene& scene) {
	Phong *red = new Phong(Colour(255.0f, 0.f, 0.f));
	Phong *blue = new Phong(Colour(0.f, 0.f, 255.0f));
	Phong *green = new Phong(Colour(0.f, 255.0f, 0.f));

	Object* backing = make_sphere(Vertex(-30.0f, 0.0f, 30.0f), 30.0f);
	backing->set_material(red);

	Object* shadow_cause = make_sphere(Vertex(10.0f, 4.0f, -12.0f), 1.0f);
	shadow_cause->set_material(blue);

  	Object* object = make_teapot();
	object->set_material(green);
	
	scene.add_object(object);
	scene.add_object(shadow_cause);
	scene.add_object(backing);
	scene.add_light(make_light());
}

// This is the entry point function to the program.
int main(int argc, char *argv[]) {
	int width = 512;
	int height = 512;
	// Create a framebuffer
	FrameBuffer* fb = new FrameBuffer(width, height);
	
	// Create a scene
	Scene scene;
	
	// Setup the scene
	build_scene_lab_4(scene);

	Vertex p_position = Vertex(10.0f, 5.0f, -30.0f);
	Vector p_lookat = Vector(-0.9f, -0.1f, 3.0f);
	Vector p_up = Vector(0.0f, 1.0f, 0.0f);
	Camera *camera = new FullCamera(0.6f, p_position, p_lookat, p_up);
	
	// Camera generates rays for each pixel in the framebuffer and records colour + depth.
	camera->render(scene,*fb);
	
	// Output the framebuffer colour and depth as two images
	fb->writeRGBFile((char *)"test4.ppm");
	// fb->writeDepthFile((char *)"depth.ppm");
	
	cerr << "\nDone.\n" << flush;
	
	// a debug check to make sure we don't leak hits.
	cerr << "Hit Pool " << Hit::pool_size << " Allocated " << Hit::allocated << "\n" << flush;
	return 0;
}
