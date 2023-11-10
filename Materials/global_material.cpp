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
#include <algorithm>

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
	refract_weight = p_refract_weight;
	ior = p_ior;
}

// reflection and recursion computation
Colour GlobalMaterial::compute_once(Ray& viewer, Hit& hit, int recurse) {
	float depth; 
	Colour result;

	if (recurse == 0) { return result; }

	// fresnel term
	float n = ior;
	float cos_i = hit.normal.dot(viewer.direction), cos_t;
	bool is_tir = false;
	fresnel(n, cos_i, cos_t, hit, is_tir);

	// reflection
	result = result + get_reflection_colour(viewer, hit, recurse);

	// is total internal reflection
	if (is_tir) return result;

	// refraction
	result = result + get_refraction_colour(viewer, hit, recurse, n, cos_i, cos_t);

	return result;
}

Colour GlobalMaterial::compute_per_light(Vector& viewer, Hit& hit, Vector& ldir) {
	// reuse Phong for diffuse and specular term
	Phong* phong = new Phong(ambient);
	return phong->compute_per_light(viewer, hit, ldir);
}

Colour GlobalMaterial::get_reflection_colour(const Ray& viewer, Hit& hit, const int recurse) {
	Ray rray;
	rray.direction = viewer.direction - ((2.0 * hit.normal.dot(viewer.direction)) * hit.normal);
	rray.position = hit.position + (0.0001f * rray.direction);

	Colour colour; float depth;
	environment->raytrace(rray, recurse-1, colour, depth);
	colour *= reflect_weight;

	return colour;
}

Colour GlobalMaterial::get_refraction_colour(Ray& viewer, Hit& hit, const int recurse, const float n, const float cos_i, const float cos_t) {
	Ray tray;
	tray.direction = ((1/n) * viewer.direction) - ((cos_t - ((1/n) * cos_i)) * hit.normal);
	tray.position = hit.position + (0.0001f * tray.direction); 

	Colour colour; float depth;
	environment->raytrace(tray, recurse-1, colour, depth);
	colour *= refract_weight;

	return colour;
}

void GlobalMaterial::fresnel(float& n, float& cos_i, float& cos_t, Hit& hit, bool& is_tir) {
	
	// angle must be positive
	cos_i = abs(cos_i);
	// flip index of refraction if exiting
	if (!hit.entering) n = 1/n;

	float kr;
	float tir = 1.f - (1/(n*n)) * (1.f - (cos_i * cos_i));

	if (tir < 0) {
		// total internal reflection
		is_tir = true;
		kr = 1.f;

	} else {
		cos_t = sqrtf(tir);
		
		// Fresnel equations
		float r_par = (ior*cos_i - cos_t) / (ior*cos_i + cos_t);
		float r_per = (cos_i - ior*cos_t) / (cos_i + ior*cos_t);
		kr = (r_par*r_par + r_per*r_per)/2.f;
	}

	float kt = 1.f - kr;
	reflect_weight = Colour(kr, kr, kr);
	refract_weight = Colour(kt, kt, kt);
}
