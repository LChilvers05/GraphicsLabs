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

 // A simple Phong based lighting model.

#include "phong_material.h"

#include <math.h>
#include <algorithm>

// The compute_once() method supplies the ambient term.
Colour Phong::compute_once(Ray& viewer, Hit& hit, int recurse) {
	return this->ambient;
}

// The compute_per_light() method supplies the diffuse and specular terms.
Colour Phong::compute_per_light(Vector& viewer, Hit& hit, Vector& ldir) {
	float d_s = get_diffuse(hit, ldir) + get_specular(viewer, hit, ldir);
	return Colour(d_s, d_s, d_s);
}

float Phong::get_diffuse(Hit& hit, const Vector& ldir) {
	Vector towards_light = -1*ldir;
	return k_d*(max(0.f, hit.normal.dot(towards_light)));
}

float Phong::get_specular(const Vector& viewer, Hit& hit, Vector& ldir) {
	Vector r = ldir - ((2.0f * (ldir.dot(hit.normal))) * hit.normal);
	return k_s * pow(max(0.f, r.dot(viewer)), 50);
}

void Phong::get_diffuse_specular_probs(float& p_d, float& p_s, Hit& hit, Ray ray) {
	p_d = k_d;
	p_s = k_s;
}
