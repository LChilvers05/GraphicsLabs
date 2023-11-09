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
	refract_weight = p_reflect_weight;
	ior = p_ior;
}

// reflection and recursion computation
Colour GlobalMaterial::compute_once(Ray& viewer, Hit& hit, int recurse) { 
	if (recurse == 0) { return ambient; }

	float depth;
	Colour reflect_colour = ambient, kr = reflect_weight;
	Colour refract_colour = ambient, kt = refract_weight;

	// the reflection ray
	Ray rray = create_reflection_ray(viewer, hit);
	environment->raytrace(rray, recurse-1, reflect_colour, depth);

	// the refraction ray
	Ray tray = create_refraction_ray(viewer, hit, ior);
	environment->raytrace(tray, recurse-1, refract_colour, depth);

	return (kt * refract_colour) + (kr * reflect_colour);
}

Ray GlobalMaterial::create_reflection_ray(const Ray& viewer, Hit& hit) {
	Ray rray;
	rray.direction = viewer.direction - ((2.0 * hit.normal.dot(viewer.direction)) * hit.normal);
	rray.position = hit.position + (0.0001f * rray.direction);
	return rray;
}

Ray GlobalMaterial::create_refraction_ray(Ray& viewer, Hit& hit, const float ior) {
	float n = ior;
	float cos_i = hit.r_normal.dot(viewer.direction);

	if (cos_i < 0) { 
		// entering object
		cos_i = -cos_i;
	} else {
		// exiting object
		hit.r_normal.negate();
		n = 1/n; 
	}

	const float k = 1.f - (1/(n*n)) * (1.f - (cos_i * cos_i));

	Ray tray;
	// return nothing if total internal reflection
	if (k < 0) { return tray; }
	const float cos_t = sqrtf(k);
	
	tray.direction = ((1/n) * viewer.direction) - ((cos_t - ((1/n) * cos_i)) * hit.r_normal);
	tray.position = hit.position + (0.0001f * tray.direction); 

	// Fresnel term
	// float r_par = (ior*cos_i - cos_t) / (ior*cos_i + cos_t);
	// float r_per = (cos_i - ior*cos_t) / (cos_i + ior*cos_t);
	// float k_r = 0.5f * ((r_par*r_par) + (r_per*r_per));
	// float k_t = 1.f - k_r;
	// refract_weight = Colour(k_t, k_t, k_t);

	return tray;
}

Colour GlobalMaterial::compute_per_light(Vector& viewer, Hit& hit, Vector& ldir) {
	// reuse Phong for diffuse and specular term
	Phong* phong = new Phong(ambient);
	return phong->compute_per_light(viewer, hit, ldir);
}
