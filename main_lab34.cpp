/****************************************************************************
 *
 * krt - Ken's Raytracer - Coursework Edition. (C) Copyright 1993-2022.
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

/* This is the top level for the program you need to create for lab three and four.
 */

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
#include "phong_material.h"
#include "falsecolour_material.h"

//classes that contain cameras, all derived from Camera
#include "simple_camera.h"
#include "full_camera.h"

using namespace std;

Sphere* make_sphere() {
	return new Sphere(Vertex(0.0f,0.0f,2.0f),1.0f);
}

PolyMesh* make_teapot() {
	PolyMesh* teapot = new PolyMesh((char *)"teapot-low.obj", false);
	Transform * transform = new Transform(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(90.0f), sin(90.0f), 0.0f,
		0.0f, -sin(90.0f), cos(90.0f), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
  	); teapot->apply_transform(*transform);
	return teapot;
}

void build_scene(Scene& scene) {
  	Object* object = make_teapot();
	FalseColour *mat = new FalseColour();
	object->set_material(mat);
	scene.add_object(object);
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
	build_scene(scene);

	// Vertex p_position = Vertex(10.0f, 20.0f, -30.0f);
	// Vector p_lookat = Vector(-1.0f, -2.0f, 3.0f);
	// Vector p_up = Vector(0.0f, 1.0f, 0.0f);
	Vertex p_position = Vertex(0.0f, 0.0f, -30.0f);
	Vector p_lookat = Vector(0.0f, 0.0f, 2.0f);
	Vector p_up = Vector(0.0f, 1.0f, 0.0f);
	Camera *camera = new FullCamera(0.6f, p_position, p_lookat, p_up);
	
	// Camera generates rays for each pixel in the framebuffer and records colour + depth.
	camera->render(scene,*fb);
	
	// Output the framebuffer colour and depth as two images
	fb->writeRGBFile((char *)"test3.ppm");
	fb->writeDepthFile((char *)"depth.ppm");
	
	cerr << "\nDone.\n" << flush;
	
	// a debug check to make sure we don't leak hits.
	cerr << "Hit Pool " << Hit::pool_size << " Allocated " << Hit::allocated << "\n" << flush;
	return 0;
}
