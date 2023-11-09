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

#include "global_material.h"
#include "phong_material.h"

#include <math.h>

GlobalMaterial::GlobalMaterial(
	Colour p_ambient,
	Environment* p_env, 
	Colour p_reflect_weight, 
	Colour p_refract_weight, 
	float p_ior
) {
	ambient = p_ambient;
	environment = p_env;
	reflect_weight = p_reflect_weight;
	refract_weight = p_reflect_weight;
	ior = p_ior;
}


// reflection and recursion computation
Colour GlobalMaterial::compute_once(Ray& viewer, Hit& hit, int recurse) {
	Colour colour = ambient; float depth;

	if (recurse == 0) { return colour; }

	// the reflection ray
	Ray rray;
	rray.direction = viewer.direction - ((2.0 * hit.normal.dot(viewer.direction)) * hit.normal);
	rray.position = hit.position + (0.0001f * rray.direction);

	//manipulates colour
	environment->raytrace(rray, recurse-1, colour, depth);

	colour *= reflect_weight;
	return colour;
}

Colour GlobalMaterial::compute_per_light(Vector& viewer, Hit& hit, Vector& ldir) {
	Phong* phong = new Phong(ambient);
	return phong->compute_per_light(viewer, hit, ldir);
}

