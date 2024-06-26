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
#include "point_light.h"

// classes that contain the materials applied to an object, all derived from Material
#include "global_material.h"
#include "phong_material.h"
#include "falsecolour_material.h"

//classes that contain cameras, all derived from Camera
#include "full_camera.h"

using namespace std;

PointLight* make_light(const Vertex& lpos, const Vector& ldir) {
	Colour lColour = Colour(255.0f, 255.0f, 255.0f);
	PointLight* light = new PointLight(lpos, ldir, lColour);
	return light;
}

void make_cornell_box(Scene& scene) {
	Phong* white = new Phong(Colour(255.f, 255.f, 255.f));
	Phong* green = new Phong(Colour(0.f, 255.f, 0.f));
	Phong* red = new Phong(Colour(255.f, 0.f, 0.f));

	PolyMesh* box = new PolyMesh((char *)"OBJs/cornell_box.obj");
	box->set_material(white);
	PolyMesh* right = new PolyMesh((char *)"OBJs/cornell_right.obj");
	right->set_material(green);
	PolyMesh* left = new PolyMesh((char *)"OBJs/cornell_left.obj");
	left->set_material(red);

    scene.add_object(box);
	scene.add_object(left);
	scene.add_object(right);
}

PolyMesh* make_teapot() {
	PolyMesh* teapot = new PolyMesh((char *)"teapot-low.obj");
	teapot->smooth_render = true;
	Transform * transform = new Transform(
		0.5, 0, 0, 0,
		0, 0, 0.5, 5,
		0, -0.5, 0, 11,
		0, 0, 0, 1
  	); 
	teapot->apply_transform(*transform);
	return teapot;
}

void build_scene(Scene& scene) {
    // cornell box
	make_cornell_box(scene);

	Phong* grey = new Phong(Colour(220.f, 220.f, 220.f));
	PolyMesh* teapot = make_teapot();
	teapot->set_material(grey);
	scene.add_object(teapot);

	Phong* blue = new Phong(Colour(0.f, 0.f, 255.f));
	Sphere* blue_sphere = new Sphere(Vertex(-6.f, 4.f, 8.f), 3.f);
	blue_sphere->set_material(blue);
	scene.add_object(blue_sphere);

    // light
    scene.add_light(make_light(Vertex(0.f, 19.5f, 5.f), Vector(0.f, -1.f, 0.f)));
}

int main(int argc, char *argv[]) {
	FrameBuffer* fb = new FrameBuffer(512, 512);
	Scene scene; build_scene(scene);

	Vertex p_position = Vertex(0.f, 10.f, -14.f);
	Vector p_lookat = Vector(0.0001f, 0.0001f, 1.f);
	Vector p_up = Vector(0.f, 1.f, 0.f);
	Camera *camera = new FullCamera(0.6f, p_position, p_lookat, p_up);

	// PASS 1: Construct Photon Map
	int photon_count = 1000000; //1000000
	scene.construct_photon_map(photon_count);

	// PASS 2: Render
	camera->render(scene, *fb);
	fb->writeRGBFile((char *)"test.ppm");
	
	cerr << "\nDone.\n" << flush;
	cerr << "Hit Pool " << Hit::pool_size << " Allocated " << Hit::allocated << "\n" << flush;
	return 0;
}