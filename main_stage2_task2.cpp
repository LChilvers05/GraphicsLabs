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
#include "csg_object.h"

// classes that contain our lights, all derived from Light
#include "directional_light.h"

// classes that contain the materials applied to an object, all derived from Material
#include "global_material.h"
#include "phong_material.h"
#include "falsecolour_material.h"

//classes that contain cameras, all derived from Camera
#include "simple_camera.h"
#include "full_camera.h"

using namespace std;

void build_scene(Scene& scene) {
    FalseColour* mat = new FalseColour();
    // ax2 + 2bxy + 2cxz + 2dx + ey2 + 2fyz + 2gy + hz2 + 2iz + j = 0

	// 1) hyperbolic paraboloid = z = x2/a2 - y2/b2
    // Quadratic* surface = new Quadratic(0.25f, 0, 0, 0, -0.25f, 0, 0, 0, -0.5f, 0);

	// 2) elliptic paraboloid = z = x2/a2 + y2/b2
    // Quadratic* surface = new Quadratic(0.25f, 0, 0, 0, 0.25f, 0, 0, 0, -0.5f, 0);

	// 3) elliptic cone = 0 = x2/a2 + y2/b2 - z2/c2
	Quadratic* surface = new Quadratic(0.25f, 0, 0, 0, 0.25f, 0, 0, -0.25f, 0, 0);
	surface->set_material(mat);

	// 4) hyperboloid of two sheets 1 = -x2/a2 - y2/b2 + z2/c2
	// Quadratic* surface = new Quadratic(-0.25f, 0, 0, 0, -0.25f, 0, 0, 0.25f, 0, -1);

	// 5) hyperboloid of one sheet 1 = x2/a2 + y2/b2 - z2/c2
	// Quadratic* surface = new Quadratic(0.25f, 0, 0, 0, 0.25f, 0, 0, -0.25f, 0, -1);

	// 6) ellipsoid = 1 = x2/a2 + y2/b2 - z2/c2
	// Quadratic* surface = new Quadratic(0.25f, 0, 0, 0, 0.25f, 0, 0, 0.25f, 0, -1);

    // surface->set_material(mat);
    // scene.add_object(surface);

	Sphere* sphere = new Sphere(Vertex(0, 0, 0), 10);
	sphere->set_material(mat);


	CSG* csg = new CSG(CSG::Mode::CSG_INTER, surface, sphere);
	csg->set_material(mat);
	
	scene.add_object(csg);
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
	Vertex p_position = Vertex(0.f, 10.f, 0.f);
	Vector p_lookat = Vector(0.f, -1.f, 0.f);
	Vector p_up = Vector(0.f, 0.f, 1.f);
	Camera *camera = new FullCamera(0.6f, p_position, p_lookat, p_up);
	
	// Camera generates rays for each pixel in the framebuffer and records colour + depth.
	camera->render(scene,*fb);
	
	// Output the framebuffer colour and depth as two images
	fb->writeRGBFile((char *)"final_csg.ppm");
	// fb->writeDepthFile((char *)"depth.ppm");
	
	cerr << "\nDone.\n" << flush;
	
	// a debug check to make sure we don't leak hits.
	cerr << "Hit Pool " << Hit::pool_size << " Allocated " << Hit::allocated << "\n" << flush;
	return 0;
}
